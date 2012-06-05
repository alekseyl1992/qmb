﻿#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "interface/document.h"
#include <QMainWindow>
#include <QMdiSubWindow>
#include <QPointer>

namespace Ui {
class MainWindow;
}

//! Класс главного окна
class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void createDocument(QString name, QString path);
public slots:
    void onCreateModel();

    void onOpenModel();

    void onSaveModel();

    void onSaveModelAs();

    void onAbout();

    void onOptions();

    void openHelp();

    void loadSettings(); //!< Загрузка настроек главного окна
    void saveSettings();  //!< Сохранение настроек главного окна

    //! срабатывает при переключении на вкладку с другой моделью и при создании новой
    void on_mdiArea_subWindowActivated(QMdiSubWindow *arg1);

    void openModel(const QString& path);

    void closeEvent(QCloseEvent *event);    

    //! Создаёт вкладку с Домашней страницей
    void createHomeWidget();

private:
    Ui::MainWindow *ui;
    Document *Doc;
    QMenu *mainMenu;
    QPointer<QMdiSubWindow> homeTab;

    //! Создаёт кнопку QMB и её меню
    void createMenuButton();
};

#endif // MAINWINDOW_H
