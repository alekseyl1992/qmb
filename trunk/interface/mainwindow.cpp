#include "interface/mainwindow.h"
#include "ui_mainwindow.h"
#include "utility/modelstorage.h"
#include "utility/simulationlog.h"
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
    ui(new Ui::MainWindow)
{
    //TODO опционально оставить весь текст на английском, затем перевети лингвистом
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("CP1251"));
    ui->setupUi(this);

    ui->mdiArea->setTabsClosable(true);
    ui->mdiArea->setTabsMovable(true);

    createMenuButton();
    createHomeWidget();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_createModel_triggered()
{
    CreateModelDialog *dialog = new CreateModelDialog(this);
    if(dialog->exec() == QDialog::Accepted)
        createDocument(dialog->name(), dialog->path());
}

void MainWindow::createDocument(QString name, QString path)
{
    Document *newDoc = new Document(this, name, path);
    QMdiSubWindow *subWindow = ui->mdiArea->addSubWindow(newDoc);
    subWindow->showMaximized();


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

void MainWindow::on_openModel_triggered()
{
    QString path = QFileDialog::getOpenFileName(this, "Открыть",
                                                    "", "QMB XML Model (*.qm)");
    if(path != "")
    {
        QString name = path.section(QRegExp("[\\\\,/]"), -1, -1);
        name.chop(3); //- .qm
        openModel(name, path);
    }
}

void MainWindow::openModel(const QString &name, const QString &path)
{
    createDocument(name, path);
}

void MainWindow::saveModel(QString path)
{
    //TODO: сохранение модели
    QMessageBox::information(this, "Сохранение модели...", path);
}

void MainWindow::on_saveModel_triggered()
{
    QString fileName =  QFileDialog::getSaveFileName(this, "Сохранить",
                                                    "", "QMB XML Model (*.qm)");
    if(fileName != "")
        saveModel(fileName);
}

void MainWindow::on_closeModel_triggered()
{
    //костыль, иначе окно созданное сразу при запуске программы нельзя закрыть
    if(!ui->mdiArea->subWindowList().empty())
        ui->mdiArea->subWindowList(QMdiArea::ActivationHistoryOrder).back()->close();
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

void MainWindow::on_mdiArea_subWindowActivated(QMdiSubWindow *arg1)
{
    if(!arg1 && ui->mdiArea->subWindowList().empty())
        createHomeWidget();
}

void MainWindow::createHomeWidget()
{
    HomeWidget *homeWidget = new HomeWidget(this);
    QMdiSubWindow *subWindow = ui->mdiArea->addSubWindow(homeWidget);
    subWindow->setWindowTitle("Добро пожаловать");
    subWindow->showMaximized();

    connect(homeWidget, SIGNAL(createModel()), this, SLOT(on_createModel_triggered()));
    connect(homeWidget, SIGNAL(openModel()), this, SLOT(on_openModel_triggered()));
    connect(homeWidget, SIGNAL(openModelByPath(QString,QString)), this, SLOT(openModel(QString,QString)));
    connect(homeWidget, SIGNAL(aboutClick()), this, SLOT(on_about_triggered()));
    connect(homeWidget, SIGNAL(exitClick()), this, SLOT(close()));
}

void MainWindow::createMenuButton()
{
    //добавляем меню перед списком табов
    QTabBar *tabBar = ui->mdiArea->findChild<QTabBar *>();
    QToolButton *menuButton = new QToolButton(tabBar);
    menuButton->setGeometry(1, 1, 48, 26);
    menuButton->setText("QMB");
    menuButton->setCheckable(true);
    menuButton->setChecked(false);
    QFont font = menuButton->font();
    font.setBold(true);
    menuButton->setFont(font);
    QMenu *mainMenu = new QMenu(this);
    mainMenu->addAction("Создать модель", this, SLOT(on_createModel_triggered()));
    mainMenu->addAction("Открыть модель", this, SLOT(on_openModel_triggered()));
    mainMenu->addSeparator();
    mainMenu->addAction("О программе", this, SLOT(on_about_triggered()));
    mainMenu->addSeparator();
    mainMenu->addAction("Выход", this, SLOT(close()));

    //отжимаем кнопку меню через dt -> 0, чтобы меню не открылось при повторном нажатии на кнопку
    /*::connect(mainMenu, SIGNAL(aboutToHide()), [menuButton]
    {
         QTimer::singleShot(100, new connect_functor_helper(menuButton, [menuButton]{ menuButton->setChecked(false); }), SLOT(signaled()));
    });*/

    std::function<void()> onClick = [mainMenu, menuButton]
    {
        if(!menuButton->isChecked())
        {
            mainMenu->exec(menuButton->mapToGlobal(QPoint(0, 26)));
            menuButton->setChecked(false);
        }
    };
    ::connect(menuButton, SIGNAL(pressed()), onClick);

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
            margin-left: 50;
        }
    )");
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    foreach(QMdiSubWindow *wnd, ui->mdiArea->subWindowList())
        if(!wnd->close())
            return event->ignore();
}
