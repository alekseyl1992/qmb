#include "interface/mainwindow.h"
#include "ui_mainwindow.h"
#include "utility/modelstorage.h"
#include "interface/homewidget.h"
#include "interface/createmodeldialog.h"
#include <QUrl>
#include <QMessageBox>
#include <QTextCodec>
#include <QMdiSubWindow>
#include <QFileDialog>
#include <QToolButton>
#include <QFont>
#include <QMenu>
#include <QTimer>

#include "utility/lsfss.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), homeTab(nullptr)
{
    //TODO опционально оставить весь текст на английском, затем перевети лингвистом
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    ui->setupUi(this);

    ui->mdiArea->setTabsClosable(true);
    ui->mdiArea->setTabsMovable(true);

    createMenuButton();
    createHomeWidget();

    //запуск с параметрами
    if(QApplication::arguments().count() > 1)
    {
        foreach(QString arg, QApplication::arguments())
        {
            if(arg.endsWith(".qm"))
                openModel(arg);
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onCreateModel()
{
    CreateModelDialog *dialog = new CreateModelDialog(this);
    if(dialog->exec() == QDialog::Accepted)
    {
        Document *newDoc = new Document(this);
        if(newDoc->createModel(dialog->name()))
        {
            QMdiSubWindow *subWindow = ui->mdiArea->addSubWindow(newDoc);
            subWindow->showMaximized();
            delete homeTab;
            homeTab = nullptr;
        }
        else //TODO а может обработку ошибок лучше в Document?
            QMessageBox::critical(this,
                "Ошибка",
                "Возникла ошибка при попытке создать модель");
    }
}

//DEPRECATED
void MainWindow::createDocument(QString name, QString path)
{
    QMessageBox::critical(this,
        "Ошибка",
        "DEPRECATED");

    /*//формируем имя документа
    QSet<QString> modelTitles;
    for(QMdiSubWindow *wnd :  ui->mdiArea->subWindowList())
        modelTitles << wnd->windowTitle();

    int modelId = -1;
    for(int i=1; i<maxOpenedModels; i++)
        if(!modelTitles.contains(QString("Модель %0").arg(i)))
        {
              modelId = i;
              break;
        }

    if(modelId != -1)
    {
        QString Name = QString("Модель %0").arg(modelId);
        //TODO вернуть меню
        Document *newDoc = new Document(this, Name);
        newDoc->scene()->setMode(ModelScene::Mode::InsertItem);

        QMdiSubWindow *subWindow = ui->mdiArea->addSubWindow(newDoc);
        subWindow->showMaximized();
        ui->mdiArea->setActiveSubWindow(subWindow);

        //добавляем кнопку старта симуляции в заголовку вкладки
        /QTabBar *tabBar = subWindow->findChild<QTabBar *>();
        //QTabBar *tabBar = ui->mdiArea->findChild<QTabBar *>();
        QToolButton *startButton = new QToolButton(tabBar);
        startButton->setIcon(QIcon(":/icons/start"));
        startButton->setGeometry(0, 0, 20, 20);
        //startButton->setText("Старт");
        //startButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        QFont font = tabBar->font();
        font.setBold(true);
        //tabBar->setFont(font);
        startButton->setAutoRaise(true);
        tabBar->setTabButton(modelId-1, QTabBar::LeftSide, startButton);/

        Doc = newDoc;
    }
    else
    {
        QMessageBox::warning(this, "Ошибка",
                     "Открыто слишком много моделей!\n"
                     "Закройте хотя бы одну перед созданием новой.");
    }*/
}

void MainWindow::onOpenModel()
{
    QString path = QFileDialog::getOpenFileName(this, "Открыть модель",
                                                QApplication::applicationDirPath() + "/models/",
                                                "QMB XML Model (*.qm)");
    if(path != "")
        openModel(path);
}

void MainWindow::openModel(const QString &path)
{
    Document *newDoc = new Document(this);
    if(newDoc->openModel(path))
    {
        QMdiSubWindow *subWindow = ui->mdiArea->addSubWindow(newDoc);
        subWindow->showMaximized();        
        delete homeTab;
        homeTab = nullptr;
    }
    else //TODO а может обработку ошибок лучше в Document?
        QMessageBox::critical(this,
            "Ошибка",
            QString("Возникла ошибка при попытке открыть модель\nПуть: %0")
                              .arg(path));
}

void MainWindow::onSaveModel()
{
    QMdiSubWindow *curTab = ui->mdiArea->currentSubWindow();
    if(curTab)
    {
        Document *curDoc = dynamic_cast<Document *>(curTab->widget());
        if(curDoc)
        {
            if(!curDoc->saveModel())
                QMessageBox::critical(this,
                    "Ошибка",
                    "Возникла ошибка при попытке сохранить модель");
        }
    }
}

void MainWindow::onSaveModelAs()
{
    QMdiSubWindow *curTab = ui->mdiArea->currentSubWindow();
    if(curTab)
    {
        Document *curDoc = dynamic_cast<Document *>(curTab->widget());
        if(curDoc)
        {

            QString path = QFileDialog::getSaveFileName(this, "Сохранение модели",
                                                        QApplication::applicationDirPath() + "/models/"+curDoc->windowTitle()+".qm",
                                         "QMB XML Model (*.qm)");
            if(path != "")
            {
                if(curDoc->saveModelAs(path))
                {
                    //включаем пункт Сохранить
                    foreach(QAction *act, mainMenu->actions())
                        if(act->objectName() == "saveAction")
                            act->setEnabled(true);
                }
                else
                    QMessageBox::critical(this,
                        "Ошибка",
                        "Возникла ошибка при попытке сохранить модель");
            }
        }
    }
}

void MainWindow::onAbout()
{
    QMessageBox::information(this, "О программе",
                             "Queueing Model Builder v0.1 beta\n\n"
                             "Разработчики:\n"
                             " Леонтьев Алексей\n"
                             " Латкин Игорь\n"
                             " Назаров Константин\n\n"
                             "\t\t\t\t2012 г.");
}

void MainWindow::on_mdiArea_subWindowActivated(QMdiSubWindow *arg1)
{
    if(!arg1 && ui->mdiArea->subWindowList().empty())
    {
        homeTab = nullptr;
        createHomeWidget();
    }

    if(arg1)
    {
        //управление пунктами сохранить/сохранить как..
        bool save, saveAs;

        //если домашняя страничка
        if(dynamic_cast<HomeWidget *>(arg1->widget()))
            save = saveAs = false;
        else if(Document *curDoc = dynamic_cast<Document *>(arg1->widget()))
        {
            saveAs = true;
            save = curDoc->isSavable();
        }

        foreach(QAction *act, mainMenu->actions())
            if(act->objectName() == "saveAction")
                act->setEnabled(save);
            else if(act->objectName() == "saveAsAction")
               act->setEnabled(saveAs);
    }
}

void MainWindow::createHomeWidget()
{
    if(!homeTab)
    {
        HomeWidget *homeWidget = new HomeWidget(this);
        homeTab = ui->mdiArea->addSubWindow(homeWidget);
        homeTab->setWindowTitle("Добро пожаловать");
        homeTab->showMaximized();

        connect(homeWidget, SIGNAL(createModel()), this, SLOT(onCreateModel()));
        connect(homeWidget, SIGNAL(openModel()), this, SLOT(onOpenModel()));
        connect(homeWidget, SIGNAL(openModelByPath(QString)), this, SLOT(openModel(QString)));
        connect(homeWidget, SIGNAL(aboutClick()), this, SLOT(onAbout()));
        connect(homeWidget, SIGNAL(exitClick()), this, SLOT(close()));
    }
}

void MainWindow::createMenuButton()
{
    //добавляем меню перед списком табов
    QTabBar *tabBar = ui->mdiArea->findChild<QTabBar *>();
    QToolButton *menuButton = new QToolButton(tabBar);
    menuButton->setGeometry(1, 1, 58, 26);
    menuButton->setText("QMB");
    menuButton->setChecked(false);
    QFont font = menuButton->font();
    font.setBold(true);
    menuButton->setFont(font);
    mainMenu = new QMenu(this);
    mainMenu->addAction("Создать модель", this, SLOT(onCreateModel()));
    mainMenu->addAction("Открыть модель", this, SLOT(onOpenModel()));
    mainMenu->addSeparator();
    QAction *saveAction = mainMenu->addAction("Сохранить модель", this, SLOT(onSaveModel()));
    saveAction->setObjectName("saveAction");
    saveAction->setEnabled(false);
    QAction *saveAsAction = mainMenu->addAction("Сохранить модель как...", this, SLOT(onSaveModelAs()));
    saveAsAction->setObjectName("saveAsAction");
    saveAsAction->setEnabled(false);
    mainMenu->addSeparator();
    mainMenu->addAction("О программе", this, SLOT(onAbout()));
    mainMenu->addSeparator();
    mainMenu->addAction("Выход", this, SLOT(close()));
    menuButton->setPopupMode(QToolButton::MenuButtonPopup);
    menuButton->setMenu(mainMenu);
    connect(menuButton, SIGNAL(clicked()), this, SLOT(createHomeWidget()));

    tabBar->setStyleSheet(R"(
        QTabBar::tab
        {
            background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,
                                      stop: 0 #E1E1E1, stop: 0.4 #DDDDDD,
                                      stop: 0.5 #D8D8D8, stop: 1.0 #D3D3D3);
            border: 2px solid #C4C4C3;
            border-bottom-color: #C2C7CB;
            border-top-left-radius: 4px;
            border-top-right-radius: 4px;
            min-width: 8ex;
            height: 21;
            padding: 2px;
        }
        QTabBar::tab:selected, QTabBar::tab:hover
        {
            background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,
                                        stop: 0 #fafafa, stop: 0.4 #f4f4f4,
                                        stop: 0.5 #e7e7e7, stop: 1.0 #fafafa);
        }

        QTabBar::tab:selected
        {
            border-color: #9B9B9B;
            border-bottom-color: #C2C7CB;
        }

        QTabBar::tab:!selected
        {
            /*margin-top: 2px;*/
        }

        QTabBar::tab:first, QTabBar::tab:only-one
        {
            margin-left: 60;
        }
    )");
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    foreach(QMdiSubWindow *wnd, ui->mdiArea->subWindowList())
        if(!wnd->close())
            return event->ignore();
}
