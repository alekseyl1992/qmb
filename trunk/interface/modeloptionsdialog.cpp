#include "modeloptionsdialog.h"
#include "ui_modeloptionsdialog.h"

ModelOptionsDialog::ModelOptionsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ModelOptionsDialog)
{
    ui->setupUi(this);
}

ModelOptionsDialog::~ModelOptionsDialog()
{
    delete ui;
}
