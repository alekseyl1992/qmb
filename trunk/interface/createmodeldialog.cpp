#include "createmodeldialog.h"
#include "ui_createmodeldialog.h"

CreateModelDialog::CreateModelDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateModelDialog)
{
    ui->setupUi(this);
    ui->name->setText("Модель 1");
    ui->path->setText(QApplication::applicationDirPath()+"/models/");
}

CreateModelDialog::~CreateModelDialog()
{
    delete ui;
}

QString CreateModelDialog::name() const
{
    return ui->name->text();
}

QString CreateModelDialog::path() const
{
    return ui->path->text() + ui->name->text() + ".qm";
}
