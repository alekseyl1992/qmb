#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "interface/document.h"
#include <QMainWindow>
#include <QMdiSubWindow>
#include <functional>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void createDocument(QString name, QString path);
public slots:
    void on_createModel_triggered();

    void on_openModel_triggered();

    void on_saveModel_triggered();

    void on_about_triggered();

    void on_closeModel_triggered();

    void on_mdiArea_subWindowActivated(QMdiSubWindow *arg1);

    void openModel(const QString& name, const QString& path);

    void saveModel(QString path);

    void closeEvent(QCloseEvent *event);

private:
    Ui::MainWindow *ui;
    Document *Doc;

    static const int maxOpenedModels = 128;

    void createMenuButton();
    void createHomeWidget();
};

#endif // MAINWINDOW_H
