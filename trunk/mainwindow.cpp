#include "mainwindow.h"
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
    //����� �������� � ��������� ������� - �������� �������

    int i = -1; //��-�� ������-���������
    foreach(QAction *act, ui->mainToolBar->actions())
            act->setData(i++);
    on_createModel_triggered();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_createModel_triggered()
{
    //����� ����� ������ �� ���� ����� ������� � ����� ����������
    Doc = new Document(this);    
    Doc->scene()->setMode(ModelScene::Mode::InsertItem);
    connect(Doc->scene(), SIGNAL(itemInserted(ModelItem *)), this, SLOT(tool_triggered()));

    Doc->setWindowTitle(QString(QString("������ ") +
                                QString::number(ui->mdiArea->subWindowList().size()+1)));
    ui->mdiArea->addSubWindow(Doc)->showMaximized();
    //Doc->code()->setText(R"<?xml version=\"1.0\" encoding="UTF-8\"?> <model name=\"������ 1\"> </model>)");
}

void MainWindow::on_openModel_triggered()
{
    QString FileName = QFileDialog::getOpenFileName(this, "�������",
                                                    "", "QMB XML Model (*.qxml)");
}

void MainWindow::on_saveModel_triggered()
{
    QString FileName =  QFileDialog::getSaveFileName(this, "���������",
                                                    "", "QMB XML Model (*.qxml)");
}

void MainWindow::on_closeModel_triggered()
{
    ui->mdiArea->closeActiveSubWindow();
}

void MainWindow::on_startSimulation_triggered()
{/*
    mymodel::generator *gen = new mymodel::generator();
    mymodel::handler *han = new mymodel::handler();
    qRegisterMetaType<mymodel::request>("request");
    connect(gen, SIGNAL(request_generated(request)), han, SLOT(handle(request)));
    gen->start();
    qDebug() << "connected" << endl;*/

    qmodel::request_generator<> gen(std::chrono::milliseconds(1000));
    qmodel::queue<> q;
    qmodel::handler<> h(std::chrono::milliseconds(5000));

    qmodel::model<> newModel;
    newModel.req_generators.push_back(gen);
    newModel.queues.push_back(q);
    newModel.handlers.push_back(h);

    newModel.link_generators_queues.push_back(qmodel::link<qmodel::request_generator<>*, qmodel::queue<>* >(&gen, &q));
    newModel.link_queues_handlers.push_back(qmodel::link<qmodel::queue<>*, qmodel::handler<>* >(&q, &h));

    qmodel::qalgorithm::simulate_start(newModel);
}

void MainWindow::on_stopSimulation_triggered()
{

}

void MainWindow::on_about_triggered()
{
    QMessageBox::information(this, "� ���������",
                             "Queueing Model Builder v0.1\n\n"
                             "������������:\n"
                             "�������� �������\n"
                             "������ �����\n"
                             "������� ����������\n\n"
                             "\t\t\t\t2012 �.");
}

void MainWindow::on_projectPage_triggered()
{
     QDesktopServices::openUrl(QUrl("http://opensvn.ru/project/qmb", QUrl::TolerantMode));
}

void MainWindow::tool_triggered()
{
    //����� �����������
    QAction *Sender = (QAction*)sender();
    foreach(QAction *act, ui->mainToolBar->actions())
        if(act != Sender)
            act->setChecked(false);

    Doc->scene()->setMode(ModelScene::Mode::InsertItem);
    Doc->scene()->setItemType(ModelItem::ItemType(Sender->data().toInt()));
}
