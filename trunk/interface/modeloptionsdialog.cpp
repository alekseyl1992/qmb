#include "modeloptionsdialog.h"
#include "ui_modeloptionsdialog.h"

ModelOptionsDialog::ModelOptionsDialog(QWidget *parent, ModelStorage *storage) :
    QDialog(parent),
    ui(new Ui::ModelOptionsDialog)
{
    ui->setupUi(this);
    if(storage)
        ui->modelName->setText(storage->getModelName());
    else
        ui->modelName->setText("Модель");
}

ModelOptionsDialog::~ModelOptionsDialog()
{
    delete ui;
}

QString ModelOptionsDialog::name() const
{
    return ui->modelName->text();
}
