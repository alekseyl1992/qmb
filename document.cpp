#include "document.h"
#include "ui_document.h"
#include "xmlhighlighter.h"
#include "simulationlog.h"
#include <QMenu>

Document::Document(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Document)
{
    ui->setupUi(this);
    new XmlHighlighter(ui->textEdit);
    Scene = new ModelScene(new QMenu(), this);
    ui->graphicsView->setScene(Scene);

    connect(&SimulationLog::Log(), SIGNAL(changed()), this, SLOT(logChanged()));
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
