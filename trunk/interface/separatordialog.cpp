#include "separatordialog.h"
#include "ui_separatordialog.h"

SeparatorDialog::SeparatorDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SeparatorDialog)
{
    ui->setupUi(this);
}

SeparatorDialog::~SeparatorDialog()
{
    delete ui;
}
