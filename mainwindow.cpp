#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qmodel/modelstorage.h"
#include "simulationlog.h"
#include "elementpropwindow.h"
#include <QDesktopServices>
#include <QUrl>
#include <QMessageBox>
#include <QTextCodec>
#include <QMdiSubWindow>
#include <QButtonGroup>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QFileDialog>

#include "qmodel\generator.h"
#include "qmodel\queue.h"
#include "qmodel\handler.h"
#include "qmodel\model.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("CP1251"));
    ui->setupUi(this);

    ui->mdiArea->setTabsClosable(true);
    ui->mdiArea->setTabsMovable(true);
    setDockOptions(AnimatedDocks);
    setCorner(Qt::TopLeftCorner, Qt::TopDockWidgetArea);
    setCorner(Qt::TopLeftCorner, Qt::LeftDockWidgetArea);
    setCorner(Qt::TopRightCorner, Qt::TopDockWidgetArea);
    setCorner(Qt::TopRightCorner, Qt::RightDockWidgetArea);
    setCorner(Qt::BottomLeftCorner, Qt::BottomDockWidgetArea);
    setCorner(Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);
    setCorner(Qt::BottomRightCorner, Qt::BottomDockWidgetArea);
    setCorner(Qt::BottomRightCorner, Qt::RightDockWidgetArea);

    //����� �������� � ��������� ������� - �������� �������

    //DEPRECATED
    int i = -1; //��-�� ������-���������
    foreach(QAction *act, ui->mainToolBar->actions())
            act->setData(i++);

    //��������� �����������
    QTreeWidgetItem *BaseTools = new QTreeWidgetItem(ui->toolsView, QStringList(QString("��������")));
    QTreeWidgetItem *item = new QTreeWidgetItem(ui->toolsView, QStringList(QString("���������")));
    //item->setData(0, 1, (int)ItemType::Generator);
    BaseTools->addChild(item);
    ui->toolsView->insertTopLevelItem(0, BaseTools);

    on_createModel_triggered();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_createModel_triggered()
{
    //����� ����� ������ �� ���� ����� ������� � ����� ����������

    //��������� ��� ���������
    QString Name = QString("������ ") +
            QString::number(ui->mdiArea->subWindowList().size()+1);

    Doc = new Document(this, ui->elementMenu, Name);
    Doc->scene()->setMode(ModelScene::Mode::InsertItem);
    //TODO ��-����� ������� �� ������, �������� �� ���� ����� ������, � connect ������
    //connect(Doc->scene(), SIGNAL(itemInserted(ModelItem *)), this, SLOT(tool_triggered()));

    ui->mdiArea->addSubWindow(Doc)->showMaximized();
}

void MainWindow::on_openModel_triggered()
{
    QString FileName = QFileDialog::getOpenFileName(this, "�������",
                                                    "", "QMB XML Model (*.qxml)");
    //TODO: ��������� ������

    //DEPRECTED
    /*qmodel::model<> *newModel = qmodel::converter::LoadQModel(FileName);
    qmodel::qalgorithm::simulation_start(*newModel);*/
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
    Doc->setActiveTab(Document::Tabs::Simulation);
    sLog.reset();
    sLog << "Simulation started\n" << endl;

    //return;
    qmodel::generator gen(1000);
    //qmodel::generator gen2(5000);
    qmodel::queue q;
    qmodel::handler h(3000);
    //qmodel::handler h2(500);

    qmodel::model newModel;
    newModel.generators.push_back(gen);
    //newModel.generators.push_back(gen2);
    newModel.queues.push_back(q);
    newModel.handlers.push_back(h);
    //newModel.handlers.push_back(h2);

    newModel.link_generators_queues.push_back(qmodel::link<qmodel::generator*, qmodel::queue* >(&gen, &q));
    //newModel.link_generators_queues.push_back(qmodel::link<qmodel::generator*, qmodel::queue* >(&gen2, &q));

    newModel.link_queues_handlers.push_back(qmodel::link<qmodel::queue*, qmodel::handler* >(&q, &h));
    //newModel.link_queues_handlers.push_back(qmodel::link<qmodel::queue*, qmodel::handler* >(&q, &h2));

    newModel.simulation_start();
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

//DEPRECATED
void MainWindow::tool_triggered()
{
    //����� �����������
    QAction *Sender = (QAction*)sender();
    foreach(QAction *act, ui->mainToolBar->actions())
        if(act != Sender)
            act->setChecked(false);

    Doc->scene()->setMode(ModelScene::Mode::InsertItem);
    //TODO ���������� ���� ���� data
    Doc->scene()->setItemType(ItemType(Sender->data().toInt()));
}



void MainWindow::on_toolsView_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    //����� �������� �����������
    Doc->scene()->setMode(ModelScene::Mode::InsertItem);
    Doc->scene()->setItemType(static_cast<ItemType>(current->data(0, 0).toInt()));
}

void MainWindow::on_deleteElement_triggered()
{
    //������� ���������� �������� � ��������������� ���������
    //TODO ������� ��� ������ � ������� � Document ��� ���� Scene
    foreach (QGraphicsItem *item, Doc->scene()->selectedItems()) {
        if (item->type() == ModelItem::Type) {
            qgraphicsitem_cast<ModelItem *>(item)->removeArrows();
        }
        Doc->scene()->removeItem(item);
    }
}

void MainWindow::on_elementProperties_triggered()
{
    ElementPropWindow *propWindow =  new ElementPropWindow(this);
    propWindow->show();
}
