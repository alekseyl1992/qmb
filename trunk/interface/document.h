#ifndef DOCUMENT_H
#define DOCUMENT_H

#include "interface/scene/modelscene.h"
#include "utility/modelstorage.h"
#include "logic/model.h"
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

    explicit Document(QWidget *parent, QString name);

    ~Document();

    ModelScene *scene();
    QTextEdit *code();

    void showLog(bool show = true);
    void setActiveTab(Tabs Tab);

    void startSimulation();
    void stopSimulation();
    bool isModified() const;
    void setModified(bool m);

public slots:
    void logChanged(QString line);
    void on_logButton_toggled(bool checked);
    void closeEvent(QCloseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void onSimulationFinished();

private slots:
    void on_toolsView_pressed(const QModelIndex &index);

    void on_startButton_clicked();

    void on_stopButton_clicked();

private:
    static const int ItemTypeRole = Qt::UserRole + 1;
    Ui::Document *ui;
    ModelScene *Scene;
    logic::ModelStorage *Storage;
    bool bSimulating;
    bool bDirty;
};

#endif // DOCUMENT_H
