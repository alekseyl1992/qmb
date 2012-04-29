#include "homewidget.h"
#include "ui_homewidget.h"

HomeWidget::HomeWidget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HomeWidget)
{
    ui->setupUi(this);
}

HomeWidget::~HomeWidget()
{
    delete ui;
}

void HomeWidget::on_createModelButton_clicked()
{
    emit createModel();
}

void HomeWidget::on_openModelButton_clicked()
{
    emit openModel();
}
