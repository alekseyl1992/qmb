#include "optionsdialog.h"
#include "ui_optionsdialog.h"
#include <QSettings>

OptionsDialog::OptionsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OptionsDialog)
{
    ui->setupUi(this);
    connect(this, SIGNAL(accepted()), this, SLOT(onAccepted()));

    //загружаем все настройки
    QSettings set;
    ui->dropShadow->setChecked(set.value("Scene/DropShadow", true).toBool());
    ui->useOpenGL->setChecked(set.value("Scene/OpenGL", true).toBool());
    ui->highLightXML->setChecked(set.value("Code/HighlightXML", true).toBool());
    ui->showStringNumbers->setChecked(set.value("Code/ShowStringNumbers", true).toBool());
    ui->fontSize->setValue(set.value("Code/FontSize", 8).toInt());
}

OptionsDialog::~OptionsDialog()
{
    delete ui;
}

void OptionsDialog::onAccepted()
{
    //сохраняем настройки
    QSettings set;
    set.setValue("Scene/DropShadow", ui->dropShadow->isChecked());
    set.setValue("Scene/OpenGL", ui->useOpenGL->isChecked());
    set.setValue("Code/HighlightXML", ui->highLightXML->isChecked());
    set.setValue("Code/ShowStringNumbers", ui->showStringNumbers->isChecked());
    set.setValue("Code/FontSize", ui->fontSize->value());
}
