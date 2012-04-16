#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "document.h"
#include <QMainWindow>
#include <QTreeWidget>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void on_createModel_triggered();

    void on_openModel_triggered();

    void on_saveModel_triggered();

    void on_startSimulation_triggered();

    void on_stopSimulation_triggered();

    void on_about_triggered();

    void on_projectPage_triggered();

    void tool_triggered();

    void on_closeModel_triggered();

    void on_deleteElement_triggered();

    void on_elementProperties_triggered();

    void on_toolsView_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);

private:
    Ui::MainWindow *ui;
    Document *Doc;
};

#endif // MAINWINDOW_H
