/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Sun 25. Mar 02:23:30 2012
**      by: Qt User Interface Compiler version 4.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMdiArea>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *createModel;
    QAction *openModel;
    QAction *saveModel;
    QAction *startSimulation;
    QAction *stopSimulation;
    QAction *about;
    QAction *projectPage;
    QAction *arrowTool;
    QAction *generatorTool;
    QAction *queueTool;
    QAction *handlerTool;
    QAction *connectTool;
    QAction *toolsCaption;
    QAction *simCaption;
    QAction *closeModel;
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QMdiArea *mdiArea;
    QMenuBar *menuBar;
    QMenu *menu;
    QMenu *menu_2;
    QMenu *menu_3;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
    QToolBar *simulationToolBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(967, 579);
        createModel = new QAction(MainWindow);
        createModel->setObjectName(QString::fromUtf8("createModel"));
        openModel = new QAction(MainWindow);
        openModel->setObjectName(QString::fromUtf8("openModel"));
        saveModel = new QAction(MainWindow);
        saveModel->setObjectName(QString::fromUtf8("saveModel"));
        startSimulation = new QAction(MainWindow);
        startSimulation->setObjectName(QString::fromUtf8("startSimulation"));
        stopSimulation = new QAction(MainWindow);
        stopSimulation->setObjectName(QString::fromUtf8("stopSimulation"));
        about = new QAction(MainWindow);
        about->setObjectName(QString::fromUtf8("about"));
        about->setMenuRole(QAction::AboutRole);
        projectPage = new QAction(MainWindow);
        projectPage->setObjectName(QString::fromUtf8("projectPage"));
        arrowTool = new QAction(MainWindow);
        arrowTool->setObjectName(QString::fromUtf8("arrowTool"));
        arrowTool->setCheckable(true);
        QIcon icon;
        icon.addFile(QString::fromUtf8("F:/\320\237\321\200\320\276\320\263\321\200\320\260\320\274\320\274\320\270\321\200\320\276\320\262\320\260\320\275\320\270\320\265/\320\223\321\200\320\260\321\204\320\270\320\272\320\260/glyphs/menu.ico"), QSize(), QIcon::Normal, QIcon::Off);
        arrowTool->setIcon(icon);
        generatorTool = new QAction(MainWindow);
        generatorTool->setObjectName(QString::fromUtf8("generatorTool"));
        generatorTool->setCheckable(true);
        queueTool = new QAction(MainWindow);
        queueTool->setObjectName(QString::fromUtf8("queueTool"));
        queueTool->setCheckable(true);
        handlerTool = new QAction(MainWindow);
        handlerTool->setObjectName(QString::fromUtf8("handlerTool"));
        handlerTool->setCheckable(true);
        connectTool = new QAction(MainWindow);
        connectTool->setObjectName(QString::fromUtf8("connectTool"));
        connectTool->setCheckable(true);
        toolsCaption = new QAction(MainWindow);
        toolsCaption->setObjectName(QString::fromUtf8("toolsCaption"));
        toolsCaption->setEnabled(false);
        QFont font;
        font.setBold(true);
        font.setUnderline(false);
        font.setWeight(75);
        font.setKerning(true);
        toolsCaption->setFont(font);
        simCaption = new QAction(MainWindow);
        simCaption->setObjectName(QString::fromUtf8("simCaption"));
        simCaption->setEnabled(false);
        QFont font1;
        font1.setBold(true);
        font1.setWeight(75);
        simCaption->setFont(font1);
        closeModel = new QAction(MainWindow);
        closeModel->setObjectName(QString::fromUtf8("closeModel"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
        centralWidget->setSizePolicy(sizePolicy);
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(0, 0, 0, 0);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        mdiArea = new QMdiArea(centralWidget);
        mdiArea->setObjectName(QString::fromUtf8("mdiArea"));
        mdiArea->setViewMode(QMdiArea::TabbedView);
        mdiArea->setDocumentMode(true);
        mdiArea->setTabShape(QTabWidget::Rounded);

        gridLayout->addWidget(mdiArea, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 967, 26));
        menu = new QMenu(menuBar);
        menu->setObjectName(QString::fromUtf8("menu"));
        menu_2 = new QMenu(menuBar);
        menu_2->setObjectName(QString::fromUtf8("menu_2"));
        menu_3 = new QMenu(menuBar);
        menu_3->setObjectName(QString::fromUtf8("menu_3"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        mainToolBar->setAllowedAreas(Qt::AllToolBarAreas);
        mainToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        MainWindow->addToolBar(Qt::LeftToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);
        simulationToolBar = new QToolBar(MainWindow);
        simulationToolBar->setObjectName(QString::fromUtf8("simulationToolBar"));
        simulationToolBar->setFloatable(true);
        MainWindow->addToolBar(Qt::LeftToolBarArea, simulationToolBar);

        menuBar->addAction(menu->menuAction());
        menuBar->addAction(menu_2->menuAction());
        menuBar->addAction(menu_3->menuAction());
        menu->addAction(createModel);
        menu->addAction(openModel);
        menu->addAction(saveModel);
        menu->addSeparator();
        menu->addAction(closeModel);
        menu_2->addAction(startSimulation);
        menu_2->addAction(stopSimulation);
        menu_3->addAction(about);
        menu_3->addAction(projectPage);
        mainToolBar->addAction(toolsCaption);
        mainToolBar->addAction(generatorTool);
        mainToolBar->addAction(queueTool);
        mainToolBar->addAction(handlerTool);
        mainToolBar->addAction(connectTool);
        simulationToolBar->addAction(simCaption);
        simulationToolBar->addAction(startSimulation);
        simulationToolBar->addAction(stopSimulation);

        retranslateUi(MainWindow);
        QObject::connect(generatorTool, SIGNAL(triggered()), MainWindow, SLOT(tool_triggered()));
        QObject::connect(queueTool, SIGNAL(triggered()), MainWindow, SLOT(tool_triggered()));
        QObject::connect(handlerTool, SIGNAL(triggered()), MainWindow, SLOT(tool_triggered()));
        QObject::connect(connectTool, SIGNAL(triggered()), MainWindow, SLOT(tool_triggered()));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Queueing Model Builder", 0, QApplication::UnicodeUTF8));
        createModel->setText(QApplication::translate("MainWindow", "\320\241\320\276\320\267\320\264\320\260\321\202\321\214", 0, QApplication::UnicodeUTF8));
        createModel->setShortcut(QApplication::translate("MainWindow", "Ctrl+N", 0, QApplication::UnicodeUTF8));
        openModel->setText(QApplication::translate("MainWindow", "\320\236\321\202\320\272\321\200\321\213\321\202\321\214", 0, QApplication::UnicodeUTF8));
        openModel->setShortcut(QApplication::translate("MainWindow", "Ctrl+O", 0, QApplication::UnicodeUTF8));
        saveModel->setText(QApplication::translate("MainWindow", "\320\241\320\276\321\205\321\200\320\260\320\275\320\270\321\202\321\214", 0, QApplication::UnicodeUTF8));
        saveModel->setShortcut(QApplication::translate("MainWindow", "Ctrl+S", 0, QApplication::UnicodeUTF8));
        startSimulation->setText(QApplication::translate("MainWindow", "\320\235\320\260\321\207\320\260\321\202\321\214", 0, QApplication::UnicodeUTF8));
        startSimulation->setShortcut(QApplication::translate("MainWindow", "F5", 0, QApplication::UnicodeUTF8));
        stopSimulation->setText(QApplication::translate("MainWindow", "\320\236\321\201\321\202\320\260\320\275\320\276\320\262\320\270\321\202\321\214", 0, QApplication::UnicodeUTF8));
        stopSimulation->setShortcut(QApplication::translate("MainWindow", "Esc", 0, QApplication::UnicodeUTF8));
        about->setText(QApplication::translate("MainWindow", "\320\236 \320\277\321\200\320\276\320\263\321\200\320\260\320\274\320\274\320\265", 0, QApplication::UnicodeUTF8));
        about->setShortcut(QApplication::translate("MainWindow", "F1", 0, QApplication::UnicodeUTF8));
        projectPage->setText(QApplication::translate("MainWindow", "\320\241\321\202\321\200\320\260\320\275\320\270\321\207\320\272\320\260 \320\277\321\200\320\276\320\265\320\272\321\202\320\260", 0, QApplication::UnicodeUTF8));
        arrowTool->setText(QApplication::translate("MainWindow", "\320\243\320\272\320\260\320\267\320\260\321\202\320\265\320\273\321\214", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        arrowTool->setToolTip(QApplication::translate("MainWindow", "\320\243\320\272\320\260\320\267\320\260\321\202\320\265\320\273\321\214", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        generatorTool->setText(QApplication::translate("MainWindow", "\320\223\320\265\320\275\320\265\321\200\320\260\321\202\320\276\321\200", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        generatorTool->setToolTip(QApplication::translate("MainWindow", "\320\223\320\265\320\275\320\265\321\200\320\260\321\202\320\276\321\200", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        queueTool->setText(QApplication::translate("MainWindow", "\320\236\321\207\320\265\321\200\320\265\320\264\321\214", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        queueTool->setToolTip(QApplication::translate("MainWindow", "\320\236\321\207\320\265\321\200\320\265\320\264\321\214", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        handlerTool->setText(QApplication::translate("MainWindow", "\320\236\320\261\321\200\320\260\320\261\320\276\321\202\321\207\320\270\320\272", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        handlerTool->setToolTip(QApplication::translate("MainWindow", "\320\236\320\261\321\201\320\273\321\203\320\266\320\270\320\262\320\260\321\216\321\211\320\270\320\271 \320\260\320\277\320\277\320\260\321\200\320\260\321\202", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        connectTool->setText(QApplication::translate("MainWindow", "\320\241\320\262\321\217\320\267\321\214", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        connectTool->setToolTip(QApplication::translate("MainWindow", "\320\233\320\270\320\275\320\270\321\217 \321\201\320\262\321\217\320\267\320\270", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        toolsCaption->setText(QApplication::translate("MainWindow", "\320\255\320\273\320\265\320\274\320\265\320\275\321\202\321\213", 0, QApplication::UnicodeUTF8));
        simCaption->setText(QApplication::translate("MainWindow", "\320\241\320\270\320\274\321\203\320\273\321\217\321\206\320\270\321\217", 0, QApplication::UnicodeUTF8));
        closeModel->setText(QApplication::translate("MainWindow", "\320\227\320\260\320\272\321\200\321\213\321\202\321\214", 0, QApplication::UnicodeUTF8));
        closeModel->setShortcut(QApplication::translate("MainWindow", "Ctrl+W", 0, QApplication::UnicodeUTF8));
        menu->setTitle(QApplication::translate("MainWindow", "\320\234\320\276\320\264\320\265\320\273\321\214", 0, QApplication::UnicodeUTF8));
        menu_2->setTitle(QApplication::translate("MainWindow", "\320\241\320\270\320\274\321\203\320\273\321\217\321\206\320\270\321\217", 0, QApplication::UnicodeUTF8));
        menu_3->setTitle(QApplication::translate("MainWindow", "?", 0, QApplication::UnicodeUTF8));
        simulationToolBar->setWindowTitle(QApplication::translate("MainWindow", "toolBar_2", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
