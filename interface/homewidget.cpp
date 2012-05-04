#include "interface/homewidget.h"
#include "ui_homewidget.h"
#include <QDesktopServices>
#include <QMessageBox>
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QRegExp>
//#include <QCleanlooksStyle>
#include "utility/lsfss.h"

HomeWidget::HomeWidget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HomeWidget)
{
    ui->setupUi(this);
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setAlignment(Qt::AlignTop);
    ui->lastModels->setLayout(layout);
    //layout->setContentsMargins(0, 0, 0, 0);

    //формируем список недавних проектов
    QFile lastModelsFile(QApplication::applicationDirPath() + "/lastModels.dat");
    lastModelsFile.open(QIODevice::ReadOnly);
    QTextStream stream(&lastModelsFile);

    if(stream.status() == QTextStream::Ok)
    {
        while(!stream.atEnd())
        {
            QString path = stream.readLine();
            QCommandLinkButton *button = new QCommandLinkButton(ui->lastModels);
            button->setText(path.section(QRegExp("[\\\\,/]"), -1, -1));
            button->setDescription(path);
            button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
            //button->setStyle(new QCleanlooksStyle());

            ::connect(button, SIGNAL(clicked()), [this, path]
            {
                emit openModelByPath(path);
            });

            layout->addWidget(button);
        }
    }
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
    QApplication::exit();
}
