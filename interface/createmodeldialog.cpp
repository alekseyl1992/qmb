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

void CreateModelDialog::on_pathButton_clicked()
{
    QFileDialog *dialog = new QFileDialog(this, "Выберите папку для создания модели", QApplication::applicationDirPath());
    dialog->setFileMode(QFileDialog::DirectoryOnly);
    QList<QUrl> sideBar = dialog->sidebarUrls();
    sideBar << QUrl::fromLocalFile(QDesktopServices::storageLocation(QDesktopServices::DesktopLocation));
    sideBar << QUrl::fromLocalFile(QDesktopServices::storageLocation(QDesktopServices::HomeLocation));
    sideBar << QUrl::fromLocalFile(QApplication::applicationDirPath());
    dialog->setSidebarUrls(sideBar);
    if(dialog->exec() == QDialog::Accepted)
        ui->path->setText(dialog->selectedFiles().at(0));
}
