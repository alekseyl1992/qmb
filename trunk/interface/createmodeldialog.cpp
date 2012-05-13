#include "createmodeldialog.h"
#include "ui_createmodeldialog.h"
#include <QFileDialog>
#include <QUrl>
#include <QDesktopServices>

CreateModelDialog::CreateModelDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateModelDialog)
{
    ui->setupUi(this);
    ui->name->setText("Модель 1");
}

CreateModelDialog::~CreateModelDialog()
{
    delete ui;
}

QString CreateModelDialog::name() const
{
    return ui->name->text();
}
