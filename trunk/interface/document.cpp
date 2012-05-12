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
    qRegisterMetaType<clock_t>("clock_t");

    //OpenGL acceleration
    ui->graphicsView->setViewport(new QGLWidget(this));

    new XmlHighlighter(ui->codeEdit);

    //создаем меню для элементов
    QMenu *itemMenu = new QMenu(this);
    itemMenu->addAction("Свойства", new connector(this, [this]
    {
        ElementPropWindow *propWindow =  new ElementPropWindow(this);
        propWindow->show();
    }), SLOT(signaled()));

    itemMenu->addAction("Удалить", new connector(this, [this]
    {
        Scene->removeSelectedItems();
    }), SLOT(signaled()), QKeySequence(Qt::Key_Delete));

    //создаём меню для лога
    logMenu = new QMenu(this);
    logMenu->addAction("Копировать выделенные", new connector(this, [this]
    {
        QClipboard *clipboard = QApplication::clipboard();
        QString strings;
        foreach(QModelIndex index, ui->simulationLog->selectionModel()->selectedRows())
            strings += logModel->item(index.row(), 0)->text().append(" | ")
                    += logModel->item(index.row(), 1)->text().append(" | ")
                    += logModel->item(index.row(), 2)->text().append("\n");

        clipboard->setText(strings);
    }), SLOT(signaled()));

    logMenu->addAction("Копировать всё", new connector(this, [this]
    {
        QClipboard *clipboard = QApplication::clipboard();
        QString strings;
        for(int i=0; i<logModel->rowCount(); i++)
            strings += logModel->item(i, 0)->text().append(" | ")
                    += logModel->item(i, 1)->text().append(" | ")
                    += logModel->item(i, 2)->text().append("\n");

        clipboard->setText(strings);
    }), SLOT(signaled()));

    logMenu->addAction("Очистить", new connector(this, [this]
    {
        logModel->removeRows(0, logModel->rowCount());
    }), SLOT(signaled()));

    Scene = new ModelScene(itemMenu, ui->graphicsView);
    ui->graphicsView->setScene(Scene);
    Storage = new ModelStorage();

    //связываем сцену с хранилищем
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
    connect(Scene, SIGNAL(wrongLink(ItemType,ItemType)),
            this, SLOT(onWrongLink(ItemType,ItemType)));

    //создаём окошко для отображения масштаба модели
//    QComboBox *box = new QComboBox(ui->graphicsView);
//    box->addItem(QIcon(), "100%");
//    QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect(this);
//    effect->setOpacity(0.5);
//    box->setGraphicsEffect(effect);

    //синхронизация записи в объект sLog и соответсвующее поле в интерфейсе
    //DEPRECATED
    //connect(&sLog, SIGNAL(changed(QString)), this, SLOT(logChanged(QString)));
    //connect(&sLog, SIGNAL(cleared()), ui->simulationLog, SLOT(clear()));

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

    /*//Прочее
    groupItem = new QStandardItem("Прочее");
    groupItem->setSelectable(false);
    //groupItem->setEnabled(false);
    groupItem->setTextAlignment(Qt::AlignCenter);
    groupItem->setEnabled(false);
    parentItem->appendRow(groupItem);
    //связь
    {
        QStandardItem *item = new QStandardItem("Связь");
        //item->setData(int(ItemType::Generator));
        item->setIcon(QIcon(":/icons/arrow"));
        //item->setSizeHint(QSize(32, 32));
        item->setDragEnabled(false);
        item->setSelectable(true);
        groupItem->appendRow(item);
    }*/

    ui->toolsView->setModel(model);
    ui->toolsView->expandAll();
    //выбираем генератор
    ui->toolsView->setCurrentIndex(generatorIndex);

    //формируем лог симуляции
    logModel = new QStandardItemModel(0, 3, this);
    logModel->setHeaderData(0, Qt::Horizontal, "Время");
    logModel->setHeaderData(1, Qt::Horizontal, "Запрос");
    logModel->setHeaderData(2, Qt::Horizontal, "Статус");
    ui->simulationLog->setModel(logModel);
    //connect(logModel, SIGNAL(rowsInserted(const QModelIndex &parent, int start, int end)), this, SLOT(scrollLog(const QModelIndex &parent, int start, int end)));

    ui->logDock->resize(0, 360);
}

Document::~Document()
{
    //релиз в порядке обратном иниту
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
    //logModel->clear();
    showLog();    
    ui->graphicsView->setEnabled(false);
    ui->progressBar->show();
    ui->startButton->hide();
    ui->stopButton->show();

    logic::model *model = Storage->getModel(true);
    connect(model, SIGNAL(simulationFinished()), this, SLOT(onSimulationFinished()));
    connect(model, SIGNAL(reqGenerated(logic::request_id, clock_t)), this, SLOT(onReqGenerated(logic::request_id, clock_t)));
    connect(model, SIGNAL(reqQueued(int,logic::request_id, clock_t)), this, SLOT(onReqQueued(int,logic::request_id, clock_t)));
    connect(model, SIGNAL(reqBeganHandling(int,logic::request_id, clock_t)), this, SLOT(onReqBeganHandling(int,logic::request_id, clock_t)));
    connect(model, SIGNAL(reqFinishedHandling(int,logic::request_id, clock_t)), this, SLOT(onReqFinishedHandling(int,logic::request_id, clock_t)));
    connect(model, SIGNAL(reqTerminated(int,logic::request_id, clock_t)), this, SLOT(onReqTerminated(int,logic::request_id, clock_t)));

    logModel->appendRow(QList<QStandardItem *>()
                        << new QStandardItem("0:00")
                        << new QStandardItem("")
                        << new QStandardItem("симуляция начата"));
    ui->simulationLog->scrollToBottom();

    model->simulation_start();
    bSimulating = true;
}

void Document::stopSimulation()
{
    int id = QMessageBox::question(
                this, windowTitle(),
                "Вы действительно хотите прервать симуляцию?",
                QMessageBox::Yes, QMessageBox::No);

    //TODO повторяющийся код
    if(id == QMessageBox::Yes)
    {
        bSimulating = false;
        Storage->getModel()->simulation_stop();
        ui->graphicsView->setEnabled(true);
        ui->progressBar->hide();
        ui->startButton->show();
        ui->stopButton->hide();

        logModel->appendRow(QList<QStandardItem *>()
                            << new QStandardItem("0:00")
                            << new QStandardItem("")
                            << new QStandardItem("симуляция прервана"));
        ui->simulationLog->scrollToBottom();

        //disconnect(Storage->getModel(), SIGNAL(simulationFinished()), this, SLOT(onSimulationFinished()));
        //disconnect(Storage->getModel(), SIGNAL(reqGenerated(request_id)), this, SLOT(onReqGenerated(logic::request_id)));
        Storage->freeModel();
    }
}

bool Document::isModified() const
{
    //TODO проверка на смену настроек симуляции
    return Scene->isModified() || code()->document()->isModified();
}

void Document::setModified(bool m)
{
    Scene->setModified(m);
}

bool Document::createModel(const QString &name, const QString &path)
{
    bool ret = Storage->createModel(name, path);
    if(ret)
    {
        setWindowTitle(Storage->getModelName());
    }

    return ret;
}

bool Document::openModel(const QString &path)
{
    bool ret = Storage->openModel(path);
    if(ret)
    {
        Storage->fillModel(Scene);
        setWindowTitle(Storage->getModelName());
    }

    return ret;
}

bool Document::saveModel()
{
    return Storage->saveModel();
}

bool Document::saveModelAs(const QString& name, const QString &path)
{
    return Storage->saveModelAs(name, path);
}

void Document::on_logButton_toggled(bool checked)
{
    showLog(checked);
}

void Document::closeEvent(QCloseEvent *event)
{
    if(bSimulating)
    {
        int id = QMessageBox::question(this, "Закрытие модели",
                                       "Симуляция не завершена, вы действительно хотите закрыть модель?",
                                       QMessageBox::Yes, QMessageBox::No);
        if(id == QMessageBox::Yes)
            stopSimulation();
        else
            return event->ignore();
    }

    if(isModified())
    {
        int id = QMessageBox::question(this, "Закрытие модели", "Сохранить модель перед закрытием?",
                                  QMessageBox::Yes, QMessageBox::No, QMessageBox::Cancel);

        //TODO сохранение модели
        if(id == QMessageBox::Yes)
        {
            //если код изменён
            if(code()->document()->isModified())
            {
                if(tryApplyCode())
                    Storage->saveModel();
            }
            else
                Storage->saveModel();
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

    logModel->appendRow(QList<QStandardItem *>()
                        << new QStandardItem("0:00")
                        << new QStandardItem("")
                        << new QStandardItem("симуляция завершена успешно"));
    ui->simulationLog->scrollToBottom();

    //disconnect(Storage->getModel(), SIGNAL(simulationFinished()), this, SLOT(onSimulationFinished()));
    //disconnect(Storage->getModel(), SIGNAL(reqGenerated(request_id)), this, SLOT(onReqGenerated(logic::request_id)));
    Storage->freeModel();

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
    //при переключении на вкладку Code, подгружаем код
    if(index == Tabs::Code)
    {
        ui->codeEdit->clear();
        ui->codeEdit->insertPlainText(Storage->getCodeString());
    }
    else //ушли с вкладки с кодом
    {
        if(ui->codeEdit->document()->isModified())
            if(tryApplyCode())
            {
                Scene->clear();
                Storage->fillModel(Scene);
            }
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
                              QString("Возникли ошибки при парсе кода:\n\tСтрока: %1\n\tОшибка: %2").arg(e.stringNum()).arg(e.text()));
        return false;
    }
}

void Document::on_simulationLog_customContextMenuRequested(const QPoint &pos)
{
    logMenu->exec(ui->simulationLog->mapToGlobal(pos));
}

void Document::onReqGenerated(const logic::request_id &reqID, clock_t event_time)
{
    const int time_size = 6;
    char buf[time_size];
    struct tm * timeinfo;
    time(&event_time);
    timeinfo = localtime ( &event_time );
    strftime (buf,time_size - 1,"%M:%S",timeinfo);

    logModel->appendRow(QList<QStandardItem *>()
                        << new QStandardItem(QString("0:00"))
                        << new QStandardItem(QString("%0:%1")
                                             .arg(reqID.__req_gen_id)
                                             .arg(reqID.__req_id))
                        << new QStandardItem("сгенерирован"));
}

void Document::onReqQueued(const int &qID, const logic::request_id &reqID, clock_t event_time)
{
    logModel->appendRow(QList<QStandardItem *>()
                        << new QStandardItem("0:00")
                        << new QStandardItem(QString("%0:%1")
                                             .arg(reqID.__req_gen_id)
                                             .arg(reqID.__req_id))
                        << new QStandardItem(QString("добавлен в очередь %0")
                                             .arg(qID)));
    ui->simulationLog->scrollToBottom();
}

void Document::onReqBeganHandling(const int &hID, const logic::request_id &reqID, clock_t event_time)
{
    logModel->appendRow(QList<QStandardItem *>()
                        << new QStandardItem("0:00")
                        << new QStandardItem(QString("%0:%1")
                                             .arg(reqID.__req_gen_id)
                                             .arg(reqID.__req_id))
                        << new QStandardItem(QString("попал в обработчик %0")
                                             .arg(hID)));
    ui->simulationLog->scrollToBottom();
}

void Document::onReqFinishedHandling(const int &hID, const logic::request_id &reqID, clock_t event_time)
{
    logModel->appendRow(QList<QStandardItem *>()
                        << new QStandardItem("0:00")
                        << new QStandardItem(QString("%0:%1")
                                             .arg(reqID.__req_gen_id)
                                             .arg(reqID.__req_id))
                        << new QStandardItem(QString("обработка завершена")));
    ui->simulationLog->scrollToBottom();
}

void Document::onReqTerminated(const int &tID, const logic::request_id &reqID, clock_t event_time)
{
    logModel->appendRow(QList<QStandardItem *>()
                        << new QStandardItem("0:00")
                        << new QStandardItem(QString("%0:%1")
                                             .arg(reqID.__req_gen_id)
                                             .arg(reqID.__req_id))
                        << new QStandardItem("изничтожен"));
    ui->simulationLog->scrollToBottom();
}

void Document::onWrongLink(ItemType fromType, ItemType toType)
{
    //TODO какие эти?
    QMessageBox::critical(this, "Ошибка", "Не возможно соединить эти элементы");
}
