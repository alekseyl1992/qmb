#include "document.h"
#include "ui_document.h"
#include "xmlhighlighter.h"

Document::Document(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Document)
{
    ui->setupUi(this);
    XmlHighlighter *Hightlighter = new XmlHighlighter(ui->textEdit);
}

Document::~Document()
{
    delete ui;
}
