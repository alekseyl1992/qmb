#include "interface/document.h"
#include "ui_document.h"
#include "utility/xmlhighlighter.h"
#include "interface/modeloptionsdialog.h"
#include "utility/common.h"
#include "utility/lsfss.h"
#include "utility/lastmodels.h"
#include "utility/validator.h"
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
#include <QToolBar>
#include <QSettings>
#include <QGraphicsProxyWidget>

Document::Document(QWidget *parent) :
    QDialog(parent), ui(new Ui::Document), bSimulating(false), bPaused(false), bClosing(false)
{
    ui->setupUi(this);
    ui->progressBar->hide();
    qRegisterMetaType<logic::request_id>("logic::request_id");
    propDialog = nullptr;
    propModel = nullptr;

    QSettings set;

    //OpenGL acceleration
    //if(set.value("Scene/OpenGL", true).toBool())
    //    ui->graphicsView->setViewport(new QGLWidget(this));

    if(set.value("Code/HighlightXML", true).toBool())
        new XmlHighlighter(ui->codeEdit);

    //создаём панели инструментов
    QHBoxLayout *toolLayout = new QHBoxLayout(ui->toolPanel);
    toolLayout->setMargin(0);
    ui->toolPanel->setLayout(toolLayout);
    QToolBar *toolBar = new QToolBar("Симуляция", ui->toolPanel);
    QAction *act = nullptr;

    act = toolBar->addAction(QIcon(":/icons/save"), "Сохранить", this, SLOT(saveModel()));
    toolBar->addSeparator();

    act = toolBar->addAction(QIcon(":/icons/undo"), "Отмена", this, SLOT(onUndoAction()));
    act->setShortcut(QKeySequence("Ctrl+Z"));
    act = toolBar->addAction(QIcon(":/icons/redo"), "Повтор", this, SLOT(onRedoAction()));
    act->setShortcut(QKeySequence("Ctrl+Y"));
    toolBar->addSeparator();

    act = toolBar->addAction(QIcon(":/icons/modelOptions"), "Настройки модели", new connector(this, [this]
    {
        ModelOptionsDialog *dialog = new ModelOptionsDialog(this, storage);
        Unimplemented();
        dialog->exec();
    }), SLOT(signaled()));
    act->setShortcut(QKeySequence("Ctrl+O"));
    toolBar->addSeparator();

    startAction = toolBar->addAction(QIcon(":/icons/start"), "Старт", this, SLOT(onStartAction()));
    stopAction = toolBar->addAction(QIcon(":/icons/stop"), "Стоп", this, SLOT(onStopAction()));
    stopAction->setEnabled(false);

    toolLayout->addWidget(toolBar);

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

    bool bDropShadow = set.value("Scene/DropShadow", true).toBool();
    scene = new ModelScene(ui->graphicsView, bDropShadow);

    ui->graphicsView->setScene(scene);
    storage = new ModelStorage();
    code = ui->codeEdit;
    code->setFontPointSize(set.value("Code/FontSize", 8).toInt());

    //связываем сцену с хранилищем
    connect(scene, SIGNAL(itemInserted(ItemType, int, QString, QPoint)),
            storage, SLOT(onItemInserted(ItemType, int, QString, QPoint)));
    connect(scene, SIGNAL(itemMoved(int, QPoint)),
            storage, SLOT(onItemMoved(int, QPoint)));
    connect(scene, SIGNAL(itemRemoved(int)),
            storage, SLOT(onItemRemoved(int)));
    connect(scene, SIGNAL(linkInserted(int, int)),
            storage, SLOT(onLinkInserted(int, int)));
    connect(scene, SIGNAL(linkRemoved(int, int)),
            storage, SLOT(onLinkRemoved(int, int)));
    connect(scene, SIGNAL(selectionChanged()), this, SLOT(onSelectionChanged()));

    connect(&Validator::inst(), SIGNAL(wrongLink(ItemType,ItemType)),
            this, SLOT(onWrongLink(ItemType,ItemType)));
    connect(&Validator::inst(), SIGNAL(modelError(logic::exceptions::LogicException&)),
            this, SLOT(onModelError(logic::exceptions::LogicException&)));

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

    for(ItemType type = ItemType::Generator; type < ItemType::Link; ++(int&)type)
    {
        QStandardItem *item = new QStandardItem(itemTypeToGUIString(type));
        item->setData(int(type));
        item->setIcon(QIcon(ModelItem(type).image()));
        groupItem->appendRow(item);
    }

    /* DEPRECATED
    //генератор
    //QModelIndex generatorIndex;
    {
        QStandardItem *item = new QStandardItem("Генератор");
        item->setData(int(ItemType::Generator));
        item->setIcon(QIcon(ModelItem(ItemType::Generator).image()));
        //item->setSizeHint(QSize(32, 32));
        groupItem->appendRow(item);
        generatorIndex = item->index();
    }
    //очередь
    {
        QStandardItem *item = new QStandardItem("Очередь");
        item->setData(int(ItemType::Queue));
        item->setIcon(QIcon(ModelItem(ItemType::Queue).image()));
        groupItem->appendRow(item);
    }
    //обработчик
    {
        QStandardItem *item = new QStandardItem("Обработчик");
        item->setData(int(ItemType::Handler));
        item->setIcon(QIcon(ModelItem(ItemType::Handler).image()));
        groupItem->appendRow(item);
    }
    //терминатор
    {
        QStandardItem *item = new QStandardItem("Терминатор");
        item->setData(int(ItemType::Terminator));
        item->setIcon(QIcon(ModelItem(ItemType::Terminator).image()));
        groupItem->appendRow(item);
    }
    //коллектор
    {
        QStandardItem *item = new QStandardItem("Терминатор");
        item->setData(int(ItemType::Terminator));
        item->setIcon(QIcon(ModelItem(ItemType::Terminator).image()));
        groupItem->appendRow(item);
    }
    //сепаратор
    {
        QStandardItem *item = new QStandardItem("Терминатор");
        item->setData(int(ItemType::Terminator));
        item->setIcon(QIcon(ModelItem(ItemType::Terminator).image()));
        groupItem->appendRow(item);
    }*/


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
    //ui->toolsView->setCurrentIndex(generatorIndex);

    //формируем лог симуляции
    logModel = new QStandardItemModel(0, 3, this);
    logModel->setHeaderData(0, Qt::Horizontal, "Время");
    logModel->setHeaderData(1, Qt::Horizontal, "Запрос");
    logModel->setHeaderData(2, Qt::Horizontal, "Статус");
    ui->simulationLog->setModel(logModel);
    ui->logDock->resize(0, 360);

    ui->propSceneSplitter->setSizes(QList<int>() << 1000 << 1);
}

Document::~Document()
{
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

void Document::setActiveTab(Tab tab)
{
    ui->tabWidget->setCurrentIndex(tab);
}

void Document::startSimulation()
{
    clearLog();    
    logic::model *model = storage->getModel(true);

    if(Validator::inst().validateModel(model))
    {
        showLog();
        bSimulating = true;

        ui->graphicsView->setEnabled(false);
        ui->progressBar->show();
        startAction->setIcon(QIcon(":/icons/pause"));
        startAction->setText("Приостановить");
        stopAction->setEnabled(true);


        connect(model, SIGNAL(reqGenerated(logic::request_id, int)), this, SLOT(onReqGenerated(logic::request_id, int)));
        connect(model, SIGNAL(reqQueued(int,logic::request_id, int)), this, SLOT(onReqQueued(int,logic::request_id, int)));
        connect(model, SIGNAL(reqBeganHandling(int,logic::request_id, int)), this, SLOT(onReqBeganHandling(int,logic::request_id, int)));
        connect(model, SIGNAL(reqFinishedHandling(int,logic::request_id, int)), this, SLOT(onReqFinishedHandling(int,logic::request_id, int)));
        connect(model, SIGNAL(reqTerminated(int,logic::request_id, int)), this, SLOT(onReqTerminated(int,logic::request_id, int)));

        connect(model, SIGNAL(simulationStarted(int)), this, SLOT(onSimulationStarted(int)));
        connect(model, SIGNAL(simulationStopped(int)), this, SLOT(onSimulationStopped(int)));
        connect(model, SIGNAL(simulationPaused(int)), this, SLOT(onSimulationPaused(int)));
        connect(model, SIGNAL(simulationRestored(int)), this, SLOT(onSimulationRestored(int)));
        connect(model, SIGNAL(simulationFinished(int)), this, SLOT(onSimulationFinished(int)));

        model->simulation_start();
    }
}

void Document::pauseSimulation()
{
    bSimulating = false;
    bPaused = true;
    storage->getModel()->simulation_pause();

    startAction->setIcon(QIcon(":/icons/continue"));
    startAction->setText("Продолжить");
    ui->progressBar->hide();
}

void Document::restoreSimulation()
{
    showLog();
    ui->progressBar->show();
    startAction->setIcon(QIcon(":/icons/pause"));
    startAction->setText("Приостановить");
    stopAction->setEnabled(true);

    bSimulating = true;
    bPaused = false;
    storage->getModel()->simulation_restore();
}

void Document::stopSimulation()
{
    //TODO повторяющийся код
    //bSimulating = false;
    storage->getModel()->simulation_stop();
    /*ui->graphicsView->setEnabled(true);
    ui->progressBar->hide();    
    startAction->setEnabled(true);
    stopAction->setEnabled(false);

    logModel->appendRow(QList<QStandardItem *>()
                        << new QStandardItem(timeToString(0)) //ИСПРАВИТЬ!!!!!!
                        << new QStandardItem("")
                        << new QStandardItem("симуляция прервана"));
    ui->simulationLog->scrollToBottom();

    //disconnect(Storage->getModel(), SIGNAL(simulationFinished()), this, SLOT(onSimulationFinished()));
    //disconnect(Storage->getModel(), SIGNAL(reqGenerated(request_id)), this, SLOT(onReqGenerated(logic::request_id)));
    storage->freeModel();*/
}

bool Document::isModified() const
{
    //TODO проверка на смену настроек симуляции
    return scene->isModified() || code->document()->isModified();
}

void Document::setModified(bool m)
{
    scene->setModified(m);
    code->document()->setModified(m);
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
    {
        bool saved = storage->saveModel();
        if(saved)
            setModified(false);
        return  saved;
    }
    else
    {
        //диалог сохранения
        QString path = QFileDialog::getSaveFileName(this, "Сохранение модели",
                                     QApplication::applicationDirPath() + "/models/" + windowTitle() + ".qm",
                                     "QMB XML Model (*.qm)");
        if(path != "" && storage->saveModelAs(path))
        {
            setModified(false);            

            //дописываем модель вверх списка последних открытых
            LastModels::getInst().add(path);
            emit beganSavable();
            return true;
        }
        else
            return false;
    }
}

bool Document::saveModelAs(const QString &path)
{
    if(storage->saveModelAs(path))
    {
        setModified(false);

        //дописываем модель вверх списка последних открытых
        LastModels::getInst().add(path);

        return true;
    }
    else
        return false;
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
        {
            bClosing = true;
            stopSimulation();
        }
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
                else
                    event->ignore();
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

void Document::onStartAction()
{
    if(ui->codeEdit->document()->isModified() &&
            !tryApplyCode())
        return;

    if(!bSimulating && !bPaused)        //запуск
        startSimulation();
    else if(!bSimulating && bPaused)    //продолжение
        restoreSimulation();
    else                                //пауза
        pauseSimulation();

}

void Document::onStopAction()
{
    /*if(QMessageBox::question(
                this, windowTitle(),
                "Вы действительно хотите прервать симуляцию?",
                QMessageBox::Yes, QMessageBox::No))*/
        stopSimulation();
}

void Document::onUndoAction()
{
    if(ui->tabWidget->currentIndex() == Tab::Scene)
    {
        if(storage->undoModel())
            storage->fillModel(scene);
    }
}

void Document::onRedoAction()
{
    if(ui->tabWidget->currentIndex() == Tab::Scene)
    {
        if(storage->redoModel())
            storage->fillModel(scene);
    }
}

void Document::on_tabWidget_currentChanged(int index)
{
    //при переключении на вкладку Code, подгружаем код
    if(index == Tab::Code)
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
    bool ret = storage->setCodeString(ui->codeEdit->document()->toPlainText());
    if(ret)
        code->document()->setModified(false);
    else
        QMessageBox::critical(this, windowTitle(), "Возникла ошибка при разборе XML");

    return ret;
}

void Document::on_simulationLog_customContextMenuRequested(const QPoint &pos)
{
    logMenu->exec(ui->simulationLog->mapToGlobal(pos));
}

void Document::onReqGenerated(const logic::request_id &reqID, int event_time)
{
    logModel->appendRow(QList<QStandardItem *>()
                        << new QStandardItem(timeToString(event_time))
                        << new QStandardItem(QString("%0")
                                             .arg(reqID.str_reqID().c_str()))
                        << new QStandardItem("Сгенерирован"));
}

void Document::onReqQueued(const int &qID, const logic::request_id &reqID, int event_time)
{
    logModel->appendRow(QList<QStandardItem *>()
                        << new QStandardItem(timeToString(event_time))
                        << new QStandardItem(QString("%0")
                                             .arg(reqID.str_reqID().c_str()))
                        << new QStandardItem(QString("Добавлен в очередь %0")
                                             .arg(qID)));
    ui->simulationLog->scrollToBottom();
}

void Document::onReqBeganHandling(const int &hID, const logic::request_id &reqID, int event_time)
{
    logModel->appendRow(QList<QStandardItem *>()
                        << new QStandardItem(timeToString(event_time))
                        << new QStandardItem(QString("%0")
                                             .arg(reqID.str_reqID().c_str()))
                        << new QStandardItem(QString("Попал в обработчик %0")
                                             .arg(hID)));
    ui->simulationLog->scrollToBottom();
}

void Document::onReqFinishedHandling(const int &hID, const logic::request_id &reqID, int event_time)
{
    logModel->appendRow(QList<QStandardItem *>()
                        << new QStandardItem(timeToString(event_time))
                        << new QStandardItem(QString("%0")
                                             .arg(reqID.str_reqID().c_str()))
                        << new QStandardItem(QString("Обработка завершена")));
    ui->simulationLog->scrollToBottom();
}

void Document::onReqTerminated(const int &tID, const logic::request_id &reqID, int event_time)
{
    logModel->appendRow(QList<QStandardItem *>()
                        << new QStandardItem(timeToString(event_time))
                        << new QStandardItem(QString("%0")
                                             .arg(reqID.str_reqID().c_str()))
                        << new QStandardItem("Уничтожен"));
    ui->simulationLog->scrollToBottom();
}

void Document::onSimulationStarted(int time)
{
    logModel->appendRow(QList<QStandardItem *>()
                        << new QStandardItem(timeToString(time))
                        << new QStandardItem("")
                        << new QStandardItem("Симуляция начата"));
    ui->simulationLog->scrollToBottom();
}

void Document::onSimulationStopped(int time)
{
    if(!bClosing)
    {
        //TODO повторяющийся код
        bSimulating = false;
        bPaused = false;
        //storage->getModel()->simulation_stop();
        ui->progressBar->hide();
        startAction->setEnabled(true);
        startAction->setIcon(QIcon(":/icons/start"));
        startAction->setText("Старт");
        stopAction->setEnabled(false);
        ui->graphicsView->setEnabled(true);

        logModel->appendRow(QList<QStandardItem *>()
                            << new QStandardItem(timeToString(time))
                            << new QStandardItem("")
                            << new QStandardItem("Симуляция прервана"));
        ui->simulationLog->scrollToBottom();

        //storage->freeModel();


        int id = QMessageBox::question(
                    this, windowTitle(),
                    "Симуляция прервана!\nПоказать статистику?",
                    QMessageBox::Yes, QMessageBox::No);


        if(id == QMessageBox::Yes)
            QMessageBox::information(this, windowTitle(), "Здесь будет отображено окно с собранной статистикой.");
    }
}

void Document::onSimulationPaused(int time)
{
    logModel->appendRow(QList<QStandardItem *>()
                        << new QStandardItem(timeToString(time))
                        << new QStandardItem("")
                        << new QStandardItem("Симуляция приостановлена"));
    ui->simulationLog->scrollToBottom();
}

void Document::onSimulationRestored(int time)
{
    logModel->appendRow(QList<QStandardItem *>()
                        << new QStandardItem(timeToString(time))
                        << new QStandardItem("")
                        << new QStandardItem("Симуляция продолжена"));
    ui->simulationLog->scrollToBottom();
}

void Document::onSimulationFinished(int time)
{
    bSimulating = false;
    bPaused = false;
    //storage->getModel()->simulation_stop();
    ui->progressBar->hide();
    startAction->setEnabled(true);
    startAction->setIcon(QIcon(":/icons/start"));
    startAction->setText("Старт");
    stopAction->setEnabled(false);
    ui->graphicsView->setEnabled(true);

    logModel->appendRow(QList<QStandardItem *>()
                        << new QStandardItem(timeToString(time))
                        << new QStandardItem("")
                        << new QStandardItem("Симуляция завершена успешно"));
    ui->simulationLog->scrollToBottom();

    //storage->freeModel();

    int id = QMessageBox::question(
                this, windowTitle(),
                "Симуляция завершена успешно!\nПоказать статистику?",
                QMessageBox::Yes, QMessageBox::No);


    if(id == QMessageBox::Yes)
        QMessageBox::information(this, windowTitle(), "Здесь будет отображено окно с собранной статистикой.");
}

void Document::onSelectionChanged()
{
    //TODO передавать сюда сразу id
    auto selectedItems = scene->selectedItems();
    if(selectedItems.count())
    {
        int id = ((ModelItem*)selectedItems[0])->id(); //устранить пиздец
        auto props = storage->getElementProperties(id);

        delete propModel;
        propModel = new QStandardItemModel(0, props.count(), this);
        QList<QStandardItem *> line;
        QStringList header;
        foreach(ModelStorage::Property prop, props)
        {
            header << prop.name;
            line << new QStandardItem(prop.value);
        }
        propModel->setHorizontalHeaderLabels(header);
        propModel->appendRow(line);

        ui->propView->setModel(propModel);
    }
}

void Document::onWrongLink(ItemType fromType, ItemType toType)
{
    QMessageBox::critical(this, "Ошибка", QString("Не возможно соединить %0 и %1.")
                          .arg(itemTypeToGUIString(fromType))
                          .arg(itemTypeToGUIString(toType)));
}

void Document::onModelError(logic::exceptions::LogicException &ex)
{
    QMessageBox::warning(this, "Ошибка модели", ex.what(), QMessageBox::Ok);
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
