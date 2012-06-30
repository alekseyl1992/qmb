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
    QWidget *list = new QWidget(ui->lastModels);
    lastModelsLayout = new QVBoxLayout(list);
    lastModelsLayout->setAlignment(Qt::AlignTop);
    list->setLayout(lastModelsLayout);
    ui->lastModels->setWidget(list);
    ui->lastModels->setWidgetResizable(true);

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
    QLayoutItem *item;
    while ((item = lastModelsLayout->takeAt(0)))
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

        lastModelsLayout->addWidget(button);
    }

    lastModelsLayout->update();
    repaint();
}
