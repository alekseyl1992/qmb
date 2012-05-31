#include "interface/document.h"
#include "ui_document.h"
#include "utility/xmlhighlighter.h"
#include "interface/elementpropwindow.h"
#include "utility/common.h"
#include "utility/lsfss.h"
#include "utility/lastmodels.h"
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
#include <QShortcut>
#include <QFileDialog>

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

    //создаем меню для элементов
    QMenu *itemMenu = new QMenu(this);
    itemMenu->addAction("Свойства", new connector(this, [this]
    {
        ElementPropWindow *propWindow =  new ElementPropWindow(this);
        propWindow->show();
    }), SLOT(signaled()));

    itemMenu->addAction("Удалить", new connector(this, [this]
    {
        scene->removeSelectedItems();
    }), SLOT(signaled()), QKeySequence(Qt::Key_Delete));

    //создаём меню для лога
    logMenu = new QMenu(this);
    std::function<void(void)> copyLog =
        [this]
        {
            QClipboard *clipboard = QApplication::clipboard();
            QString strings;
            foreach(QModelIndex index, ui->simulationLog->selectionModel()->selectedRows())
                strings += logModel->item(index.row(), 0)->text().append(" | ")
                        += logModel->item(index.row(), 1)->text().append(" | ")
                        += logModel->item(index.row(), 2)->text().append("\n");

            clipboard->setText(strings);
        };

    logMenu->addAction("Копировать выделенные", new connector(this, copyLog), SLOT(signaled()));
    QShortcut *copy = new QShortcut(QKeySequence(tr("Ctrl+C", "Log|Copy")),
                              ui->simulationLog);
    ::connect(copy, SIGNAL(activated()), copyLog);


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
    logMenu->addAction("Очистить", this, SLOT(clearLog()));

    scene = new ModelScene(itemMenu, ui->graphicsView);
    ui->graphicsView->setScene(scene);
    storage = new ModelStorage();
    code = ui->codeEdit;

    //связываем сцену с хранилищем
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


    ui->logDock->resize(0, 360);
}

Document::~Document()
{
    //релиз в порядке обратном иниту
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
                        << new QStandardItem("симуляция начата"));
    ui->simulationLog->scrollToBottom();

    model->simulation_start();
    bSimulating = true;
}

void Document::stopSimulation()
{
    //TODO повторяющийся код
    bSimulating = false;
    int time = storage->getModel()->simulation_stop();
    ui->graphicsView->setEnabled(true);
    ui->progressBar->hide();
    ui->startButton->show();
    ui->stopButton->hide();

    logModel->appendRow(QList<QStandardItem *>()
                        << new QStandardItem(timeToString(time))
                        << new QStandardItem("")
                        << new QStandardItem("симуляция прервана"));
    ui->simulationLog->scrollToBottom();

    //disconnect(Storage->getModel(), SIGNAL(simulationFinished()), this, SLOT(onSimulationFinished()));
    //disconnect(Storage->getModel(), SIGNAL(reqGenerated(request_id)), this, SLOT(onReqGenerated(logic::request_id)));
    storage->freeModel();
}

bool Document::isModified() const
{
    //TODO проверка на смену настроек симуляции
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

        //дописываем модель вверх списка последних открытых
        LastModels::getInst().add(path);
    }

    return ret;
}

bool Document::saveModel()
{
    if(isSavable()) //если уже сохраняли
        return storage->saveModel();
    else //только при закрытии программы
    {
        //диалог сохранения
        QString path = QFileDialog::getSaveFileName(this, "Сохранение модели",
                                     QApplication::applicationDirPath() + "/models/" + windowTitle() + ".qm",
                                     "QMB XML Model (*.qm)");
        if(path != "")
            return storage->saveModelAs(path);
        else
            return false;
    }
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
            if(code->document()->isModified())
            {
                if(tryApplyCode())
                    saveModel();
            }
            else
                saveModel();
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
                        << new QStandardItem("симуляция завершена успешно"));
    ui->simulationLog->scrollToBottom();

    storage->freeModel();

    int id = QMessageBox::question(
                this, windowTitle(),
                "Симуляция завершена успешно!\nПоказать статистику?",
                QMessageBox::Yes, QMessageBox::No);


    if(id == QMessageBox::Yes)
        QMessageBox::information(this, windowTitle(), "Здесь будет отображено окно с собранной статистикой.");
}

void Document::clearLog()
{
    logModel->removeRows(0, logModel->rowCount());
}

void Document::on_toolsView_pressed(const QModelIndex &index)
{
    //смена текущего инструмента
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
                "Вы действительно хотите прервать симуляцию?",
                QMessageBox::Yes, QMessageBox::No))
        stopSimulation();
}

void Document::on_tabWidget_currentChanged(int index)
{
    //при переключении на вкладку Code, подгружаем код
    if(index == Tabs::Code)
    {
        ui->codeEdit->clear();
        ui->codeEdit->insertPlainText(storage->getCodeString());
    }
    else //ушли с вкладки с кодом
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
                              QString("Возникли ошибки при парсе кода:\n\tСтрока: %1\n\tОшибка: %2").arg(e.stringNum()).arg(e.text()));
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
                        << new QStandardItem("сгенерирован"));
}

void Document::onReqQueued(const int &qID, const logic::request_id &reqID, int event_time)
{
    logModel->appendRow(QList<QStandardItem *>()
                        << new QStandardItem(timeToString(event_time))
                        << new QStandardItem(QString("%0:%1")
                                             .arg(reqID.__req_gen_id)
                                             .arg(reqID.__req_id))
                        << new QStandardItem(QString("добавлен в очередь %0")
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
                        << new QStandardItem(QString("попал в обработчик %0")
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
                        << new QStandardItem(QString("обработка завершена")));
    ui->simulationLog->scrollToBottom();
}

void Document::onReqTerminated(const int &tID, const logic::request_id &reqID, int event_time)
{
    logModel->appendRow(QList<QStandardItem *>()
                        << new QStandardItem(timeToString(event_time))
                        << new QStandardItem(QString("%0:%1")
                                             .arg(reqID.__req_gen_id)
                                             .arg(reqID.__req_id))
                        << new QStandardItem("уничтожен"));
    ui->simulationLog->scrollToBottom();
}

void Document::onWrongLink(ItemType fromType, ItemType toType)
{
    QMessageBox::critical(this, "Ошибка", QString("Не возможно соединить %0 и %1.")
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
