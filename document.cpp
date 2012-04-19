#include "document.h"
#include "ui_document.h"
#include "xmlhighlighter.h"
#include "simulationlog.h"
#include "common.h"
#include <QMenu>
#include <QComboBox>
#include <QGraphicsOpacityEffect>
#include <QPushButton>
#include <QLabel>
#include <QStandardItemModel>

Document::Document(QWidget *parent, QMenu *menu, QString name) :
    QDialog(parent),
    ui(new Ui::Document)
{
    ui->setupUi(this);
    setWindowTitle(name);

    new XmlHighlighter(ui->textEdit);
    Scene = new ModelScene(menu, ui->graphicsView);
    ui->graphicsView->setScene(Scene);
    Storage = new qmodel::ModelStorage(name);

    //связываяем сцену с хранилищем
    connect(Scene, SIGNAL(itemInserted(ItemType, int, QPoint)),
            Storage, SLOT(onItemInserted(ItemType, int, QPoint)));
    connect(Scene, SIGNAL(itemMoved(int, QPoint)),
            Storage, SLOT(onItemMoved(int, QPoint)));
    connect(Scene, SIGNAL(itemRemoved(int)),
            Storage, SLOT(onItemRemoved(int)));

    //создаём окошко для отображения масштаба модели
//    QComboBox *box = new QComboBox(ui->graphicsView);
//    box->addItem(QIcon(), "100%");
//    QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect(this);
//    effect->setOpacity(0.5);
//    box->setGraphicsEffect(effect);

    //синхронизация записи в объект sLog и соответсвующее поле в интерфейсе
    connect(&sLog, SIGNAL(changed()), this, SLOT(logChanged()));

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
    QStandardItem *groupItem = new QStandardItem("Основные");
    groupItem->setSelectable(false);
    //groupItem->set
    parentItem->appendRow(groupItem);
    //генератор
    QModelIndex generatorIndex;
    {
        QStandardItem *item = new QStandardItem("Генератор");
        item->setData(int(ItemType::Generator));
        groupItem->appendRow(item);
        generatorIndex = item->index();
    }
    //очередь
    {
        QStandardItem *item = new QStandardItem("Очередь");
        item->setData(int(ItemType::Queue));
        groupItem->appendRow(item);
    }
    //обработчик
    {
        QStandardItem *item = new QStandardItem("Обработчик");
        item->setData(int(ItemType::Handler));
        groupItem->appendRow(item);
    }
    //терминатор
    {
        QStandardItem *item = new QStandardItem("Терминатор");
        item->setData(int(ItemType::Terminator));
        groupItem->appendRow(item);
    }

    ui->toolsView->setModel(model);
    ui->toolsView->expandAll();
    //выбираем генератор
    ui->toolsView->setCurrentIndex(generatorIndex);
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

void Document::logChanged()
{
    //лучше переделать замену текста, на добавление
    ui->simulationLog->setText(SimulationLog::Log().text());
}

void Document::on_logButton_toggled(bool checked)
{
    showLog(checked);
}

void Document::on_toolsView_clicked(const QModelIndex &index)
{
    //смена текущего инструмента
    ItemType itemType = (ItemType)index.data(ItemTypeRole).toInt();
    if(itemType != ItemType::InvalidItem)
    {
        Scene->setMode(ModelScene::Mode::InsertItem); //TODO ?
        Scene->setItemType(itemType);
    }
}
