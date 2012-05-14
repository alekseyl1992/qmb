#ifndef DOCUMENT_H
#define DOCUMENT_H

#include "interface/scene/modelscene.h"
#include "utility/modelstorage.h"
#include "logic/model.h"
#include "logic/request_id.h"
#include <QDialog>
#include <QTextEdit>
#include <QTreeWidgetItem>
#include <QScrollBar>
#include <QMenu>
#include <QStandardItemModel>

namespace Ui {
class Document;
}

//class ModelStorage; //forward declaration

class Document : public QDialog
{
    Q_OBJECT
    
public:
    enum Tabs {Model, Code, Simulation};

    explicit Document(QWidget *parent);

    ~Document();

    void showLog(bool show = true);
    void setActiveTab(Tabs Tab);

    void startSimulation();
    void stopSimulation();
    bool isModified() const;
    void setModified(bool m);

    bool createModel(const QString& name);
    bool openModel(const QString& path);
    bool saveModel();
    bool saveModelAs(const QString& path);
    bool isSavable() const;

public slots:
    void on_logButton_toggled(bool checked);
    void closeEvent(QCloseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void onSimulationFinished(int event_time);
    void clearLog();

private slots:
    void on_toolsView_pressed(const QModelIndex &index);

    void on_startButton_clicked();

    void on_stopButton_clicked();

    void on_tabWidget_currentChanged(int index);

    void on_simulationLog_customContextMenuRequested(const QPoint &pos);

    void onReqGenerated(const logic::request_id& reqID, int event_time);
    void onReqQueued(const int& qID, const logic::request_id& reqID, int event_time);
    void onReqBeganHandling(const int& hID, const logic::request_id& reqID, int event_time);
    void onReqFinishedHandling(const int& hID, const logic::request_id& reqID, int event_time);
    void onReqTerminated(const int& tID, const logic::request_id& reqID, int event_time);
    void onWrongLink(ItemType fromType, ItemType toType);

private:
    static const int ItemTypeRole = Qt::UserRole + 1;
    Ui::Document *ui;
    ModelScene *scene;
    ModelStorage *storage;
    QTextEdit *code;
    QMenu *logMenu;
    QStandardItemModel *logModel;
    bool bSimulating;

    bool tryApplyCode();
    QString timeToString(int time);
};

#endif // DOCUMENT_H