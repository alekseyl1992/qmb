#include "document.h"
#include "ui_document.h"
#include "xmlhighlighter.h"
#include "simulationlog.h"
#include "elementpropwindow.h"
#include "common.h"
#include "lsfss.h"
#include <QMenu>
#include <QComboBox>
#include <QGraphicsOpacityEffect>
#include <QPushButton>
#include <QLabel>
#include <QStandardItemModel>
#include <QMessageBox>
#include <QProgressBar>
#include <QtOpenGL/QGLWidget>

Document::Document(QWidget *parent, QString name) :
    QDialog(parent), bSimulating(false), ui(new Ui::Document)
{
    ui->setupUi(this);
    setWindowTitle(name);
    ui->progressBar->hide();
    ui->stopButton->hide();

    //ахалай-махалай, OpenGL accelearation подключай
    ui->graphicsView->setViewport(new QGLWidget(this));

    new XmlHighlighter(ui->textEdit);
    QMenu *itemMenu = new QMenu(this);
    itemMenu->addAction("Свойства", new connect_functor_helper(this, [this]
    {
        ElementPropWindow *propWindow =  new ElementPropWindow(this);
        propWindow->show();
    }), SLOT(signaled()));

    itemMenu->addAction("Удалить", new connect_functor_helper(this, [this]
    {
        foreach (QGraphicsItem *item, Scene->selectedItems())
        {
            if (item->type() == ModelItem::Type)
            {
                qgraphicsitem_cast<ModelItem *>(item)->removeArrows();
            }
            Scene->removeItem(item);
        }
    }), SLOT(signaled()), QKeySequence(Qt::Key_Delete));

    Scene = new ModelScene(itemMenu, ui->graphicsView);
    ui->graphicsView->setScene(Scene);
    Storage = new qmodel::ModelStorage(name);

    //связываяем сцену с хранилищем
    connect(Scene, SIGNAL(itemInserted(ItemType, int, QPoint)),
            Storage, SLOT(onItemInserted(ItemType, int, QPoint)));
    connect(Scene, SIGNAL(itemMoved(ItemType, int, QPoint)),
            Storage, SLOT(onItemMoved(ItemType, int, QPoint)));
    connect(Scene, SIGNAL(itemRemoved(ItemType, int)),
            Storage, SLOT(onItemRemoved(ItemType, int)));

    //создаём окошко для отображения масштаба модели
//    QComboBox *box = new QComboBox(ui->graphicsView);
//    box->addItem(QIcon(), "100%");
//    QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect(this);
//    effect->setOpacity(0.5);
//    box->setGraphicsEffect(effect);

    //синхронизация записи в объект sLog и соответсвующее поле в интерфейсе
    connect(&sLog, SIGNAL(changed(QString)), this, SLOT(logChanged(QString)));
    connect(&sLog, SIGNAL(cleared()), ui->simulationLog, SLOT(clear()));

    //убираем заголовок палитры
    QWidget *nullHeader = new QWidget(this);
    ui->toolsDock->setTitleBarWidget(nullHeader);

    //меняем вид заголовка лога
    /*QWidget *head = new QWidget(ui->logDock);
    QLabel *label = new QLabel("Лог симуляции");
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

     //добавляем инструменты
    QStandardItemModel *model = new QStandardItemModel(ui->toolsView);
    QStandardItem *parentItem = model->invisibleRootItem();

    //основные
    QStandardItem *groupItem = new QStandardItem("Элементы");
    groupItem->setSelectable(false);
    //groupItem->setEnabled(false);
    groupItem->setTextAlignment(Qt::AlignCenter);
    groupItem->setEnabled(false);
    parentItem->appendRow(groupItem);

    //генератор
    QModelIndex generatorIndex;
    {
        QStandardItem *item = new QStandardItem("Генератор");
        item->setData(int(ItemType::Generator));
        item->setIcon(QIcon(ModelItem(ItemType::Generator, -1, nullptr).image()));
        //item->setSizeHint(QSize(32, 32));
        groupItem->appendRow(item);
        generatorIndex = item->index();
    }
    //очередь
    {
        QStandardItem *item = new QStandardItem("Очередь");
        item->setData(int(ItemType::Queue));
        item->setIcon(QIcon(ModelItem(ItemType::Queue, -1, nullptr).image()));
        groupItem->appendRow(item);
    }
    //обработчик
    {
        QStandardItem *item = new QStandardItem("Обработчик");
        item->setData(int(ItemType::Handler));
        item->setIcon(QIcon(ModelItem(ItemType::Handler, -1, nullptr).image()));
        groupItem->appendRow(item);
    }
    //терминатор
    {
        QStandardItem *item = new QStandardItem("Терминатор");
        item->setData(int(ItemType::Terminator));
        item->setIcon(QIcon(ModelItem(ItemType::Terminator, -1, nullptr).image()));
        groupItem->appendRow(item);
    }

    ui->toolsView->setModel(model);
    ui->toolsView->expandAll();
    //выбираем генератор
    ui->toolsView->setCurrentIndex(generatorIndex);

    ui->logDock->resize(0, 30);
}

Document::~Document()
{
    //релиз в порядке обратном иниту
    delete Storage;
    delete ui;
}

ModelScene *Document::scene()
{
    return Scene;
}

QTextEdit *Document::code()
{
    return ui->textEdit;
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
    ui->progressBar->show();
    ui->startButton->hide();
    ui->stopButton->show();

    qmodel::model *model = Storage->getModel(true);
    connect(model, SIGNAL(simulationFinished()), this, SLOT(onSimulationFinished()));
    model->simulation_start();
    bSimulating = true;
}

void Document::stopSimulation()
{
    int id = QMessageBox::question(
                this, windowTitle(),
                "Вы действительно хотите прервать симуляцию?",
                QMessageBox::Yes, QMessageBox::No);

    if(id == QMessageBox::Yes)
    {
        Storage->getModel()->simulation_stop();
        ui->progressBar->hide();
        ui->startButton->show();
        ui->stopButton->hide();
    }
}

void Document::logChanged(QString line)
{
    //лучше переделать замену текста, на добавление
    ui->simulationLog->addItem(line);
    ui->simulationLog->verticalScrollBar()->setValue(
                ui->simulationLog->verticalScrollBar()->maximum());
}

void Document::on_logButton_toggled(bool checked)
{
    showLog(checked);
}

void Document::closeEvent(QCloseEvent *event)
{
    int id = QMessageBox::question(this, "Закрытие модели", "Сохранить модель перед закрытием?",
                          "Да", "Нет", "Отмена");
    //TODO
    if(id == 2) //отмена
        event->ignore();
}

void Document::keyPressEvent(QKeyEvent *event)
{
   if(event->key() != Qt::Key_Escape)
       QDialog::keyPressEvent(event);
}

void Document::onSimulationFinished()
{    
    ui->progressBar->hide();
    ui->startButton->show();
    ui->stopButton->hide();

    int id = QMessageBox::question(
                this, windowTitle(),
                "Симуляция завершена успешно!\nПоказать статистику?",
                QMessageBox::Yes, QMessageBox::No);


    if(id == QMessageBox::Yes)
        QMessageBox::information(this, windowTitle(), "Здесь будет отображено окно с собранной статистикой.");
}

void Document::on_toolsView_pressed(const QModelIndex &index)
{
    //смена текущего инструмента
    ItemType itemType = (ItemType)index.data(ItemTypeRole).toInt();
    Scene->setMode(ModelScene::Mode::InsertItem); //TODO ?
    Scene->setItemType(itemType);
}

void Document::on_startButton_clicked()
{
    startSimulation();
}

void Document::on_stopButton_clicked()
{
    stopSimulation();
}
