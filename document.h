#ifndef DOCUMENT_H
#define DOCUMENT_H

#include "modelscene.h"
#include "qmodel/modelstorage.h"
#include "qmodel/model.h"
#include <QDialog>
#include <QTextEdit>
#include <QTreeWidgetItem>
#include <QScrollBar>

namespace Ui {
class Document;
}

//class ModelStorage; //forward declaration

class Document : public QDialog
{
    Q_OBJECT
    
public:
    enum Tabs {Model, Code, Simulation};

    explicit Document(QWidget *parent, QMenu *menu, QString name);

    ~Document();

    ModelScene *scene();
    QTextEdit *code();

    void showLog(bool show = true);
    void setActiveTab(Tabs Tab);

    void startSimulation();
public slots:
    void logChanged();
    void on_logButton_toggled(bool checked);
    void closeEvent(QCloseEvent *event);
    void onSimulationFinished();

private slots:
    void on_toolsView_pressed(const QModelIndex &index);

private:
    static const int ItemTypeRole = Qt::UserRole + 1;
    Ui::Document *ui;
    ModelScene *Scene;
    qmodel::ModelStorage *Storage;
};

#endif // DOCUMENT_H
