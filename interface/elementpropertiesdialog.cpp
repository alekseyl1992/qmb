#include "elementpropertiesdialog.h"
#include "ui_elementpropertiesdialog.h"
#include <QList>

ElementPropertiesDialog::ElementPropertiesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ElementPropertiesDialog)
{
    ui->setupUi(this);
    propModel = new QStandardItemModel(0, 3, this);
    propModel->setHeaderData(0, Qt::Horizontal, "Тип");
    propModel->setHeaderData(1, Qt::Horizontal, "Имя");
    propModel->setHeaderData(2, Qt::Horizontal, "Период генерации");
    propModel->appendRow(QList<QStandardItem *>()
                        << new QStandardItem("Генератор")
                        << new QStandardItem("Генератор 1")
                        << new QStandardItem("10 сек."));


    ui->propView->setModel(propModel);
}

ElementPropertiesDialog::~ElementPropertiesDialog()
{
    delete ui;
}
