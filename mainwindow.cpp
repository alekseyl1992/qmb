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

    //формируем имя документа
    QString Name = QString("Модель ") +
            QString::number(ui->mdiArea->subWindowList().size()+1);

    Doc = new Document(this, ui->elementMenu, Name);
    Doc->scene()->setMode(ModelScene::Mode::InsertItem);

    QMdiSubWindow *subWindow = ui->mdiArea->addSubWindow(Doc);
    subWindow->showMaximized();
    ui->mdiArea->setActiveSubWindow(subWindow);
}

void MainWindow::on_openModel_triggered()
{
    QString FileName = QFileDialog::getOpenFileName(this, "Открыть",
                                                    "", "QMB XML Model (*.qxml)");
    //TODO: подгрузка модели

    //DEPRECTED
    /*qmodel::model<> *newModel = qmodel::converter::LoadQModel(FileName);
    qmodel::qalgorithm::simulation_start(*newModel);*/
}

void MainWindow::on_saveModel_triggered()
{
    QString FileName =  QFileDialog::getSaveFileName(this, "Сохранить",
                                                    "", "QMB XML Model (*.qxml)");
}

void MainWindow::on_closeModel_triggered()
{
    if(ui->mdiArea->activeSubWindow() == nullptr)
        ui->mdiArea->activateNextSubWindow();

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
    Doc->showLog();
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
     QDesktopServices::openUrl(QUrl("http://code.google.com/p/qmb/", QUrl::TolerantMode));
}

void MainWindow::on_deleteElement_triggered()
{
    //удаляем выделенные элементы и ассоциированные стрелочки
    //TODO вынести всю работу с моделью в Document или даже Scene
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
