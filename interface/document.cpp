#include "interface/document.h"
#include "ui_document.h"
#include "utility/xmlhighlighter.h"
#include "interface/elementpropwindow.h"
#include "utility/common.h"
#include "utility/lsfss.h"
#include <QMenu>
#include <QComboBox>
#include <QGraphicsOpacityEffect>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>
#include <QProgressBar>
#include <QtOpenGL/QGLWidget>
#include <QClipboard>
#include <QListWidgetItem>
#include <QStandardItem>

Document::Document(QWidget *parent) :
    QDialog(parent), ui(new Ui::Document), bSimulating(false)
{
    ui->setupUi(this);
    ui->progressBar->hide();
    ui->stopButton->hide();
    qRegisterMetaType<logic::request_id>("logic::request_id");

    //OpenGL acceleration
    ui->graphicsView->setViewport(new QGLWidget(this));

    new XmlHighlighter(ui->codeEdit);

    //������� ���� ��� ���������
    QMenu *itemMenu = new QMenu(this);
    itemMenu->addAction("��������", new connector(this, [this]
    {
        ElementPropWindow *propWindow =  new ElementPropWindow(this);
        propWindow->show();
    }), SLOT(signaled()));

    itemMenu->addAction("�������", new connector(this, [this]
    {
        scene->removeSelectedItems();
    }), SLOT(signaled()), QKeySequence(Qt::Key_Delete));

    //������ ���� ��� ����
    logMenu = new QMenu(this);
    logMenu->addAction("���������� ����������", new connector(this, [this]
    {
        QClipboard *clipboard = QApplication::clipboard();
        QString strings;
        foreach(QModelIndex index, ui->simulationLog->selectionModel()->selectedRows())
            strings += logModel->item(index.row(), 0)->text().append(" | ")
                    += logModel->item(index.row(), 1)->text().append(" | ")
                    += logModel->item(index.row(), 2)->text().append("\n");

        clipboard->setText(strings);
    }), SLOT(signaled()));

    logMenu->addAction("���������� ��", new connector(this, [this]
    {
        QClipboard *clipboard = QApplication::clipboard();
        QString strings;
        for(int i=0; i<logModel->rowCount(); i++)
            strings += logModel->item(i, 0)->text().append(" | ")
                    += logModel->item(i, 1)->text().append(" | ")
                    += logModel->item(i, 2)->text().append("\n");

        clipboard->setText(strings);
    }), SLOT(signaled()));
    logMenu->addAction("��������", this, SLOT(clearLog()));

    scene = new ModelScene(itemMenu, ui->graphicsView);
    ui->graphicsView->setScene(scene);
    storage = new ModelStorage();
    code = ui->codeEdit;

    //��������� ����� � ����������
    connect(scene, SIGNAL(itemInserted(ItemType, int, QPoint)),
            storage, SLOT(onItemInserted(ItemType, int, QPoint)));
    connect(scene, SIGNAL(itemMoved(ItemType, int, QPoint)),
            storage, SLOT(onItemMoved(ItemType, int, QPoint)));
    connect(scene, SIGNAL(itemRemoved(ItemType, int)),
            storage, SLOT(onItemRemoved(ItemType, int)));
    connect(scene, SIGNAL(linkInserted(ItemType,int,ItemType,int)),
            storage, SLOT(onLinkInserted(ItemType,int,ItemType,int)));
    connect(scene, SIGNAL(linkRemoved(ItemType,int,ItemType,int)),
            storage, SLOT(onLinkRemoved(ItemType,int,ItemType,int)));
    connect(scene, SIGNAL(wrongLink(ItemType,ItemType)),
            this, SLOT(onWrongLink(ItemType,ItemType)));
    ::connect(scene, SIGNAL(undoRequested()),
            [scene, storage]
            {
                if(storage->undoModel())
                    storage->fillModel(scene);
            });

    //������ ������ ��� ����������� �������� ������
//    QComboBox *box = new QComboBox(ui->graphicsView);
//    box->addItem(QIcon(), "100%");
//    QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect(this);
//    effect->setOpacity(0.5);
//    box->setGraphicsEffect(effect);

    //������������� ������ � ������ sLog � �������������� ���� � ����������
    //DEPRECATED
    //connect(&sLog, SIGNAL(changed(QString)), this, SLOT(logChanged(QString)));
    //connect(&sLog, SIGNAL(cleared()), ui->simulationLog, SLOT(clear()));

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

    //��������� ��� ���������
    logModel = new QStandardItemModel(0, 3, this);
    logModel->setHeaderData(0, Qt::Horizontal, "�����");
    logModel->setHeaderData(1, Qt::Horizontal, "������");
    logModel->setHeaderData(2, Qt::Horizontal, "������");
    ui->simulationLog->setModel(logModel);
    //connect(logModel, SIGNAL(rowsInserted(const QModelIndex &parent, int start, int end)), this, SLOT(scrollLog(const QModelIndex &parent, int start, int end)));

    ui->logDock->resize(0, 360);
}

Document::~Document()
{
    //����� � ������� �������� �����
    delete storage;
    delete ui;
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
    clearLog();
    showLog();    
    ui->graphicsView->setEnabled(false);
    ui->progressBar->show();
    ui->startButton->hide();
    ui->stopButton->show();

    logic::model *model = storage->getModel(true);
    connect(model, SIGNAL(simulationFinished(int event_time)), this, SLOT(onSimulationFinished(int event_time)));
    connect(model, SIGNAL(reqGenerated(logic::request_id, int)), this, SLOT(onReqGenerated(logic::request_id, int)));
    connect(model, SIGNAL(reqQueued(int,logic::request_id, int)), this, SLOT(onReqQueued(int,logic::request_id, int)));
    connect(model, SIGNAL(reqBeganHandling(int,logic::request_id, int)), this, SLOT(onReqBeganHandling(int,logic::request_id, int)));
    connect(model, SIGNAL(reqFinishedHandling(int,logic::request_id, int)), this, SLOT(onReqFinishedHandling(int,logic::request_id, int)));
    connect(model, SIGNAL(reqTerminated(int,logic::request_id, int)), this, SLOT(onReqTerminated(int,logic::request_id, int)));

    logModel->appendRow(QList<QStandardItem *>()
                        << new QStandardItem("00:00.000")
                        << new QStandardItem("")
                        << new QStandardItem("��������� ������"));
    ui->simulationLog->scrollToBottom();

    model->simulation_start();
    bSimulating = true;
}

void Document::stopSimulation()
{
    //TODO ������������� ���
    bSimulating = false;
    int time = storage->getModel()->simulation_stop();
    ui->graphicsView->setEnabled(true);
    ui->progressBar->hide();
    ui->startButton->show();
    ui->stopButton->hide();

    logModel->appendRow(QList<QStandardItem *>()
                        << new QStandardItem(timeToString(time))
                        << new QStandardItem("")
                        << new QStandardItem("��������� ��������"));
    ui->simulationLog->scrollToBottom();

    //disconnect(Storage->getModel(), SIGNAL(simulationFinished()), this, SLOT(onSimulationFinished()));
    //disconnect(Storage->getModel(), SIGNAL(reqGenerated(request_id)), this, SLOT(onReqGenerated(logic::request_id)));
    storage->freeModel();
}

bool Document::isModified() const
{
    //TODO �������� �� ����� �������� ���������
    return scene->isModified() || code->document()->isModified();
}

void Document::setModified(bool m)
{
    scene->setModified(m);
}

bool Document::createModel(const QString &name)
{
    bool ret = storage->createModel(name);
    if(ret)
    {
        setWindowTitle(storage->getModelName());
    }

    return ret;
}

bool Document::openModel(const QString &path)
{
    bool ret = storage->openModel(path);
    if(ret)
    {
        storage->fillModel(scene);
        setWindowTitle(storage->getModelName());
    }

    return ret;
}

bool Document::saveModel()
{
    return storage->saveModel();
}

bool Document::saveModelAs(const QString &path)
{
    return storage->saveModelAs(path);
}

bool Document::isSavable() const
{
    return storage->getCurrentPath() != "";
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
    else if(isModified())
    {
        int id = QMessageBox::question(this, "�������� ������", "��������� ������ ����� ���������?",
                                  QMessageBox::Yes, QMessageBox::No, QMessageBox::Cancel);

        //TODO ���������� ������
        if(id == QMessageBox::Yes)
        {
            //���� ��� ������
            if(code->document()->isModified())
            {
                if(tryApplyCode())
                    storage->saveModel();
            }
            else
                storage->saveModel();
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

void Document::onSimulationFinished(int event_time)
{    
    bSimulating = false;
    ui->progressBar->hide();
    ui->startButton->show();
    ui->stopButton->hide();
    ui->graphicsView->setEnabled(true);

    logModel->appendRow(QList<QStandardItem *>()
                        << new QStandardItem(timeToString(event_time))
                        << new QStandardItem("")
                        << new QStandardItem("��������� ��������� �������"));
    ui->simulationLog->scrollToBottom();

    //disconnect(Storage->getModel(), SIGNAL(simulationFinished()), this, SLOT(onSimulationFinished()));
    //disconnect(Storage->getModel(), SIGNAL(reqGenerated(request_id)), this, SLOT(onReqGenerated(logic::request_id)));
    storage->freeModel();

    int id = QMessageBox::question(
                this, windowTitle(),
                "��������� ��������� �������!\n�������� ����������?",
                QMessageBox::Yes, QMessageBox::No);


    if(id == QMessageBox::Yes)
        QMessageBox::information(this, windowTitle(), "����� ����� ���������� ���� � ��������� �����������.");
}

void Document::clearLog()
{
    logModel->removeRows(0, logModel->rowCount());
}

void Document::on_toolsView_pressed(const QModelIndex &index)
{
    //����� �������� �����������
    ItemType itemType = (ItemType)index.data(ItemTypeRole).toInt();
    scene->setMode(ModelScene::Mode::InsertItem); //TODO ?
    scene->setItemType(itemType);
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
    if(QMessageBox::question(
                this, windowTitle(),
                "�� ������������� ������ �������� ���������?",
                QMessageBox::Yes, QMessageBox::No))
        stopSimulation();
}

void Document::on_tabWidget_currentChanged(int index)
{
    //��� ������������ �� ������� Code, ���������� ���
    if(index == Tabs::Code)
    {
        ui->codeEdit->clear();
        ui->codeEdit->insertPlainText(storage->getCodeString());
    }
    else //���� � ������� � �����
    {
        if(ui->codeEdit->document()->isModified())
            if(tryApplyCode())
                storage->fillModel(scene);
    }
}

bool Document::tryApplyCode()
{
    try
    {
        storage->setCodeString(ui->codeEdit->document()->toPlainText());
        code->document()->setModified(false);
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

void Document::onReqGenerated(const logic::request_id &reqID, int event_time)
{
    logModel->appendRow(QList<QStandardItem *>()
                        << new QStandardItem(timeToString(event_time))
                        << new QStandardItem(QString("%0:%1")
                                             .arg(reqID.__req_gen_id)
                                             .arg(reqID.__req_id))
                        << new QStandardItem("������������"));
}

void Document::onReqQueued(const int &qID, const logic::request_id &reqID, int event_time)
{
    logModel->appendRow(QList<QStandardItem *>()
                        << new QStandardItem(timeToString(event_time))
                        << new QStandardItem(QString("%0:%1")
                                             .arg(reqID.__req_gen_id)
                                             .arg(reqID.__req_id))
                        << new QStandardItem(QString("�������� � ������� %0")
                                             .arg(qID)));
    ui->simulationLog->scrollToBottom();
}

void Document::onReqBeganHandling(const int &hID, const logic::request_id &reqID, int event_time)
{
    logModel->appendRow(QList<QStandardItem *>()
                        << new QStandardItem(timeToString(event_time))
                        << new QStandardItem(QString("%0:%1")
                                             .arg(reqID.__req_gen_id)
                                             .arg(reqID.__req_id))
                        << new QStandardItem(QString("����� � ���������� %0")
                                             .arg(hID)));
    ui->simulationLog->scrollToBottom();
}

void Document::onReqFinishedHandling(const int &hID, const logic::request_id &reqID, int event_time)
{
    logModel->appendRow(QList<QStandardItem *>()
                        << new QStandardItem(timeToString(event_time))
                        << new QStandardItem(QString("%0:%1")
                                             .arg(reqID.__req_gen_id)
                                             .arg(reqID.__req_id))
                        << new QStandardItem(QString("��������� ���������")));
    ui->simulationLog->scrollToBottom();
}

void Document::onReqTerminated(const int &tID, const logic::request_id &reqID, int event_time)
{
    logModel->appendRow(QList<QStandardItem *>()
                        << new QStandardItem(timeToString(event_time))
                        << new QStandardItem(QString("%0:%1")
                                             .arg(reqID.__req_gen_id)
                                             .arg(reqID.__req_id))
                        << new QStandardItem("���������"));
    ui->simulationLog->scrollToBottom();
}

void Document::onWrongLink(ItemType fromType, ItemType toType)
{
    QMessageBox::critical(this, "������", QString("�� �������� ��������� %0 � %1.")
                          .arg(itemTypeToString(fromType))
                          .arg(itemTypeToString(toType)));
}

QString Document::timeToString(int time)
{
    QString str;
    int msec = 0,
        sec  = 0,
        min  = 0;

    msec = time;
    sec = msec/1000;
    min = sec/60;

    msec -= sec*1000;
    sec -= min*60;

    str.sprintf("%02d:%02d.%03d", min, sec, msec);

    return str;
}
