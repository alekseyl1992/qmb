#include "interface/homewidget.h"
#include "ui_homewidget.h"
#include <QRegExp>
#include "utility/lsfss.h"
#include "utility/lastmodels.h"

HomeWidget::HomeWidget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HomeWidget)
{
    ui->setupUi(this);
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setAlignment(Qt::AlignTop);
    ui->lastModels->setLayout(layout);

    //"подписка" на обновления списка последних моделей
    connect(&LastModels::getInst(), SIGNAL(changed()), this, SLOT(updateLastModelsList()));
    updateLastModelsList();
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

void HomeWidget::on_aboutButton_clicked()
{
    emit aboutClick();
}

void HomeWidget::on_exitButton_clicked()
{
    emit exitClick();
}

void HomeWidget::on_helpButton_clicked()
{
    emit helpClick();
}

void HomeWidget::updateLastModelsList()
{
    QLayout *layout = ui->lastModels->layout();
    QLayoutItem *item;
    while ((item = layout->takeAt(0)))
    {
        delete item->widget();
        delete item;
    }

    //формируем список недавних проектов
    foreach(QString path, LastModels::getInst().getList())
    {
        QString name = path.section(QRegExp("[\\\\,/]"), -1, -1);
        QCommandLinkButton *button = new QCommandLinkButton(ui->lastModels);
        button->setText(name);
        button->setDescription(path);
        button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        button->setIcon(QIcon(":/icons/logo"));
        button->setIconSize(QSize(32, 32));

        ::connect(button, SIGNAL(clicked()), [this, path]
        {
            emit openModelByPath(path);
        });

        layout->addWidget(button);
    }

    layout->update();
    repaint();
}
