#include "document.h"
#include "ui_document.h"
#include "xmlhighlighter.h"
#include <QMenu>

Document::Document(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Document)
{
    ui->setupUi(this);
    new XmlHighlighter(ui->textEdit);
    Scene = new ModelScene(new QMenu(), this);
    ui->graphicsView->setScene(Scene);
}

Document::~Document()
{
    delete ui;
}

QTextEdit *Document::code()
{
    return ui->textEdit;
}
