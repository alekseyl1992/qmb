#include "interface/document.h"
#include "ui_document.h"
#include "utility/xmlhighlighter.h"
#include "utility/simulationlog.h"
#include "interface/elementpropwindow.h"
#include "utility/common.h"
#include "utility/lsfss.h"
#include <QMenu>
#include <QComboBox>
#include <QGraphicsOpacityEffect>
#include <QPushButton>
#include <QLabel>
#include <QStandardItemModel>
#include <QMessageBox>
#include <QProgressBar>
#include <QtOpenGL/QGLWidget>
#include <QClipboard>
#include <QListWidgetItem>

Document::Document(QWidget *parent, QString name) :
    QDialog(parent), ui(new Ui::Document), bSimulating(false)
{
    ui->setupUi(this);
    setWindowTitle(name);
    ui->progressBar->hide();
    ui->stopButton->hide();

    //OpenGL acceleration
    ui->graphicsView->setViewport(new QGLWidget(this));

    new XmlHighlighter(ui->codeEdit);

    //������� ���� ��� ���������
    QMenu *itemMenu = new QMenu(this);
    itemMenu->addAction("��������", new connect_functor_helper(this, [this]
    {
        ElementPropWindow *propWindow =  new ElementPropWindow(this);
        propWindow->show();
    }), SLOT(signaled()));

    itemMenu->addAction("�������", new connect_functor_helper(this, [this]
    {
        Scene->removeSelectedItems();
    }), SLOT(signaled()), QKeySequence(Qt::Key_Delete));

    //������ ���� ��� ����
    logMenu = new QMenu(this);
    logMenu->addAction("���������� ����������", new connect_functor_helper(this, [this]
    {
        QClipboard *clipboard = QApplication::clipboard();
        QString strings;
        foreach(QListWidgetItem *item, ui->simulationLog->selectedItems())
        strings += item->text() + "\n";
        clipboard->setText(strings);
    }), SLOT(signaled()));

    logMenu->addAction("���������� ��", new connect_functor_helper(this, [this]
    {
        QClipboard *clipboard = QApplication::clipboard();
        QString strings;
        foreach(QListWidgetItem *item, ui->simulationLog->findItems("*", Qt::MatchWildcard))
            strings += item->text() + "\n";
        clipboard->setText(strings);
    }), SLOT(signaled()));

    logMenu->addAction("��������", new connect_functor_helper(this, [this]
    {
        ui->simulationLog->clear();
    }), SLOT(signaled()));

    Scene = new ModelScene(itemMenu, ui->graphicsView);
    ui->graphicsView->setScene(Scene);
    Storage = new ModelStorage(name);

    //��������� ����� � ����������
    connect(Scene, SIGNAL(itemInserted(ItemType, int, QPoint)),
            Storage, SLOT(onItemInserted(ItemType, int, QPoint)));
    connect(Scene, SIGNAL(itemMoved(ItemType, int, QPoint)),
            Storage, SLOT(onItemMoved(ItemType, int, QPoint)));
    connect(Scene, SIGNAL(itemRemoved(ItemType, int)),
            Storage, SLOT(onItemRemoved(ItemType, int)));
    connect(Scene, SIGNAL(linkInserted(ItemType,int,ItemType,int)),
            Storage, SLOT(onLinkInserted(ItemType,int,ItemType,int)));
    connect(Scene, SIGNAL(linkRemoved(ItemType,int,ItemType,int)),
            Storage, SLOT(onLinkRemoved(ItemType,int,ItemType,int)));

    //������ ������ ��� ����������� �������� ������
//    QComboBox *box = new QComboBox(ui->graphicsView);
//    box->addItem(QIcon(), "100%");
//    QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect(this);
//    effect->setOpacity(0.5);
//    box->setGraphicsEffect(effect);

    //������������� ������ � ������ sLog � �������������� ���� � ����������
    connect(&sLog, SIGNAL(changed(QString)), this, SLOT(logChanged(QString)));
    connect(&sLog, SIGNAL(cleared()), ui->simulationLog, SLOT(clear()));

    //������� ��������� �������
    QWidget *nullHeader = new QWidget(this);
    ui->toolsDock->setTitleBarWidget(nullHeader);

    //������ ��� ��������� ����
    /*QWidget *head = new QWidget(ui->logDock);
    QLabel *label = new QLabel("��� ���������");
    QPushButton *button = new QPushButton("_");
    QRect geometry = button->geometry();
    geometry.setWidth(geometry.height());
    button->setGeometry(geometry);
    button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    QHBoxLayout *layout = new QHBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(label);
    layout->addWidget(button);

    head->setLayout(layout);
    ui->logDock->setTitleBarWidget(head);*/

    //��������� �����������
    QStandardItemModel *model = new QStandardItemModel(ui->toolsView);
    QStandardItem *parentItem = model->invisibleRootItem();

    //��������
    QStandardItem *groupItem = new QStandardItem("��������");
    groupItem->setSelectable(false);
    //groupItem->setEnabled(false);
    groupItem->setTextAlignment(Qt::AlignCenter);
    groupItem->setEnabled(false);
    parentItem->appendRow(groupItem);

    //���������
    QModelIndex generatorIndex;
    {
        QStandardItem *item = new QStandardItem("���������");
        item->setData(int(ItemType::Generator));
        item->setIcon(QIcon(ModelItem(ItemType::Generator, -1, nullptr).image()));
        //item->setSizeHint(QSize(32, 32));
        groupItem->appendRow(item);
        generatorIndex = item->index();
    }
    //�������
    {
        QStandardItem *item = new QStandardItem("�������");
        item->setData(int(ItemType::Queue));
        item->setIcon(QIcon(ModelItem(ItemType::Queue, -1, nullptr).image()));
        groupItem->appendRow(item);
    }
    //����������
    {
        QStandardItem *item = new QStandardItem("����������");
        item->setData(int(ItemType::Handler));
        item->setIcon(QIcon(ModelItem(ItemType::Handler, -1, nullptr).image()));
        groupItem->appendRow(item);
    }
    //����������
    {
        QStandardItem *item = new QStandardItem("����������");
        item->setData(int(ItemType::Terminator));
        item->setIcon(QIcon(ModelItem(ItemType::Terminator, -1, nullptr).image()));
        groupItem->appendRow(item);
    }

    /*//������
    groupItem = new QStandardItem("������");
    groupItem->setSelectable(false);
    //groupItem->setEnabled(false);
    groupItem->setTextAlignment(Qt::AlignCenter);
    groupItem->setEnabled(false);
    parentItem->appendRow(groupItem);
    //�����
    {
        QStandardItem *item = new QStandardItem("�����");
        //item->setData(int(ItemType::Generator));
        item->setIcon(QIcon(":/icons/arrow"));
        //item->setSizeHint(QSize(32, 32));
        item->setDragEnabled(false);
        item->setSelectable(true);
        groupItem->appendRow(item);
    }*/

    ui->toolsView->setModel(model);
    ui->toolsView->expandAll();
    //�������� ���������
    ui->toolsView->setCurrentIndex(generatorIndex);

    ui->logDock->resize(0, 30);
}

Document::~Document()
{
    //����� � ������� �������� �����
    delete Storage;
    delete ui;
}

ModelScene *Document::scene() const
{
    return Scene;
}

QTextEdit *Document::code() const
{
    return ui->codeEdit;
}

void Document::showLog(bool show)
{
    if(show)
        ui->logDock->show();
    else
        ui->logDock->hide();
}

void Document::setActiveTab(Document::Tabs Tab)
{
    ui->tabWidget->setCurrentIndex(Tab);
}

void Document::startSimulation()
{
    ui->simulationLog->clear();
    showLog();    
    ui->graphicsView->setEnabled(false);
    ui->progressBar->show();
    ui->startButton->hide();
    ui->stopButton->show();

    logic::model *model = Storage->getModel(true);
    connect(model, SIGNAL(simulationFinished()), this, SLOT(onSimulationFinished()));
    model->simulation_start();
    bSimulating = true;
}

void Document::stopSimulation()
{
    int id = QMessageBox::question(
                this, windowTitle(),
                "�� ������������� ������ �������� ���������?",
                QMessageBox::Yes, QMessageBox::No);

    if(id == QMessageBox::Yes)
    {
        bSimulating = false;
        Storage->getModel()->simulation_stop();
        ui->graphicsView->setEnabled(true);
        ui->progressBar->hide();
        ui->startButton->show();
        ui->stopButton->hide();
    }
}

bool Document::isModified() const
{
    //TODO �������� �� ����� �������� ���������
    return Scene->isModified() || code()->document()->isModified();
}

void Document::setModified(bool m)
{
    Scene->setModified(m);
}

void Document::logChanged(QString line)
{
    ui->simulationLog->addItem(line);
    ui->simulationLog->scrollToBottom();
}

void Document::on_logButton_toggled(bool checked)
{
    showLog(checked);
}

void Document::closeEvent(QCloseEvent *event)
{
    if(bSimulating)
    {
        int id = QMessageBox::question(this, "�������� ������",
                                       "��������� �� ���������, �� ������������� ������ ������� ������?",
                                       QMessageBox::Yes, QMessageBox::No);
        if(id == QMessageBox::Yes)
            stopSimulation();
        else
            return event->ignore();
    }

    if(isModified())
    {
        int id = QMessageBox::question(this, "�������� ������", "��������� ������ ����� ���������?",
                                  QMessageBox::Yes, QMessageBox::No, QMessageBox::Cancel);

        //TODO ���������� ������
        if(id == QMessageBox::Yes)
        {
            //���� ��� ������
            if(code()->document()->isModified())
            {
                if(tryApplyCode())
                    Storage->saveModel(QString());
            }
            else
                Storage->saveModel(QString());
        }
        else if(id == QMessageBox::Cancel)
            event->ignore();
    }
}

void Document::keyPressEvent(QKeyEvent *event)
{
   if(event->key() != Qt::Key_Escape)
       QDialog::keyPressEvent(event);
}

void Document::onSimulationFinished()
{    
    bSimulating = false;
    ui->progressBar->hide();
    ui->startButton->show();
    ui->stopButton->hide();
    ui->graphicsView->setEnabled(true);
    Storage->freeModel();

    int id = QMessageBox::question(
                this, windowTitle(),
                "��������� ��������� �������!\n�������� ����������?",
                QMessageBox::Yes, QMessageBox::No);


    if(id == QMessageBox::Yes)
        QMessageBox::information(this, windowTitle(), "����� ����� ���������� ���� � ��������� �����������.");
}

void Document::on_toolsView_pressed(const QModelIndex &index)
{
    //����� �������� �����������
    ItemType itemType = (ItemType)index.data(ItemTypeRole).toInt();
    Scene->setMode(ModelScene::Mode::InsertItem); //TODO ?
    Scene->setItemType(itemType);
}

void Document::on_startButton_clicked()
{
    if(ui->codeEdit->document()->isModified())
    {
        if(tryApplyCode())
            startSimulation();
    }
    else
        startSimulation();
}

void Document::on_stopButton_clicked()
{
    stopSimulation();
}

void Document::on_tabWidget_currentChanged(int index)
{
    //��� ������������ �� ������� Code, ���������� ���
    if(index == Tabs::Code)
    {
        ui->codeEdit->clear();
        ui->codeEdit->insertPlainText(Storage->getCodeString());
    }
    else //���� � ������� � �����
    {
        if(ui->codeEdit->document()->isModified())
            if(tryApplyCode())
                Storage->fillModel(Scene);
    }
}

bool Document::tryApplyCode()
{
    try
    {
        Storage->setCodeString(ui->codeEdit->document()->toPlainText());
        code()->document()->setModified(false);
        return true;
    }
    catch(const ModelStorage::ParseException& e)
    {
        QMessageBox::critical(this, windowTitle(),
                              QString("�������� ������ ��� ����� ����:\n\t������: %1\n\t������: %2").arg(e.stringNum()).arg(e.text()));
        return false;
    }
}

void Document::on_simulationLog_customContextMenuRequested(const QPoint &pos)
{
    logMenu->exec(ui->simulationLog->mapToGlobal(pos));
}
