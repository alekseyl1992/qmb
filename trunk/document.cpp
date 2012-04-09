#include "document.h"
#include "ui_document.h"
#include "xmlhighlighter.h"
#include "simulationlog.h"
#include <QMenu>
#include <QComboBox>
#include <QGraphicsOpacityEffect>

Document::Document(QWidget *parent, QMenu *menu) :
    QDialog(parent),
    ui(new Ui::Document)
{
    ui->setupUi(this);
    new XmlHighlighter(ui->textEdit);
    Scene = new ModelScene(menu, ui->graphicsView);
    ui->graphicsView->setScene(Scene);

    //создаём окошко для отображения масштаба модели
    QComboBox *box = new QComboBox(ui->graphicsView);
    box->addItem(QIcon(), "100%");
    QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect(this);
    effect->setOpacity(0.5);
    box->setGraphicsEffect(effect);


    //синхронизация записи в объект sLog и соответсвующее поле в интерфейсе
    connect(&sLog, SIGNAL(changed()), this, SLOT(logChanged()));
}

Document::~Document()
{
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

void Document::setActiveTab(Document::Tabs Tab)
{
    ui->tabWidget->setCurrentIndex(Tab);
}

void Document::logChanged()
{
    //лучше переделать не на замену текста, а на добавление
    ui->simulationLog->setText(SimulationLog::Log().text());
}
