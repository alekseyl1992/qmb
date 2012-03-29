#include "mainwindow.h"
#include "document.h"
#include "ui_mainwindow.h"
#include "main.h"
#include "mymodel/model.h"
#include <QDesktopServices>
#include <QUrl>
#include <QMessageBox>
#include <QTextCodec>
#include <QMdiSubWindow>
#include <QToolButton>
#include <QButtonGroup>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("CP1251"));
    ui->setupUi(this);

    ui->mdiArea->setTabsClosable(true);
    ui->mdiArea->setTabsMovable(true);
    //будет вынесено в отдельную функцию - создание проекта
    on_createModel_triggered();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_createModel_triggered()
{
    //здесь будет запрос на ввод имени проекта и места сохранения
    Document *Doc = new Document(this);
    Doc->setWindowTitle(QString(QString("Модель ") +
                                QString::number(ui->mdiArea->subWindowList().size()+1)));
    ui->mdiArea->addSubWindow(Doc)->showMaximized();
}

void MainWindow::on_openModel_triggered()
{
    QString FileName = QFileDialog::getOpenFileName(this, "Открыть",
                                                    "", "QMB XML Model (*.qxml)");
}

void MainWindow::on_saveModel_triggered()
{
    QString FileName =  QFileDialog::getSaveFileName(this, "Сохранить",
                                                    "", "QMB XML Model (*.qxml)");
}

void MainWindow::on_closeModel_triggered()
{
    ui->mdiArea->closeActiveSubWindow();
}

void MainWindow::on_startSimulation_triggered()
{/*
    qmodel::request_generator<int> gen;
    qmodel::queue<int> q;
    qmodel::handler<int> h;

    std::thread th(generating, &gen, &q);
    std::thread th2(add_to_handler, &h);
    th.join();
    th2.join();*/

    mymodel::generator *gen = new mymodel::generator();
    mymodel::handler *han = new mymodel::handler();
    qRegisterMetaType<mymodel::request>("request");
    connect(gen, SIGNAL(request_generated(request)), han, SLOT(handle(request)));
    gen->start();
    qDebug() << "connected" << endl;
}

void MainWindow::on_stopSimulation_triggered()
{

}

void MainWindow::on_about_triggered()
{
    QMessageBox::information(this, "О программе",
                             "Queueing Model Builder v0.1\n\n"
                             "Разработчики:\n"
                             "Леонтьев Алексей\n"
                             "Латкин Игорь\n"
                             "Назаров Константин\n\n"
                             "\t\t\t\t2012 г.");
}

void MainWindow::on_projectPage_triggered()
{
     QDesktopServices::openUrl(QUrl("http://opensvn.ru/project/qmb", QUrl::TolerantMode));
}

void MainWindow::tool_triggered()
{
    //смена инструмента
    QAction *Sender = (QAction*)sender();
    foreach(QAction *act, ui->mainToolBar->actions())
        if(act != Sender)
            act->setChecked(false);

    QMessageBox::information(this, "Выбран инструмент", Sender->text());
}
