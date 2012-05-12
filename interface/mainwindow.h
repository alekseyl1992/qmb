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
    void onCreateModel();

    void onOpenModel();

    void onSaveModel();

    void onSaveModelAs();

    void onAbout();

    void on_mdiArea_subWindowActivated(QMdiSubWindow *arg1);

    void openModel(const QString& path);

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
