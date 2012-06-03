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

//! Класс окна, которое создаётся на каждой вкладке.
/*!
 * В этом окне находиться большая часть интерфейса, в том числе Сцена и список Элементов.
 */

class Document : public QDialog
{
    Q_OBJECT
    
public:
    enum Tab {Scene, Code, Options}; //!< перечисление вкладок

    explicit Document(QWidget *parent);

    ~Document();

    void showLog(bool show = true);
    void setActiveTab(Tab tab);

    void startSimulation();
    void stopSimulation();
    bool isModified() const;
    void setModified(bool m);

    //! сохранялась ли когда-либо модель
    bool isSavable() const;

public slots:
    void on_logButton_toggled(bool checked);
    void closeEvent(QCloseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void onSimulationFinished(int event_time);
    void clearLog();

    bool createModel(const QString& name);
    bool openModel(const QString& path);
    bool saveModel();
    bool saveModelAs(const QString& path);

private slots:
    //! срабатывает при начале перетаскивания элемента с палитры на Сцену
    void on_toolsView_pressed(const QModelIndex &index);

    void onStartAction();
    void onStopAction();
    void onUndoAction();
    void onRedoAction();

    //! срабатывает при смене текущей вкладки Tabs
    void on_tabWidget_currentChanged(int index);

    void on_simulationLog_customContextMenuRequested(const QPoint &pos);

    //слоты для связи с model
    void onReqGenerated(const logic::request_id& reqID, int event_time);
    void onReqQueued(const int& qID, const logic::request_id& reqID, int event_time);
    void onReqBeganHandling(const int& hID, const logic::request_id& reqID, int event_time);
    void onReqFinishedHandling(const int& hID, const logic::request_id& reqID, int event_time);
    void onReqTerminated(const int& tID, const logic::request_id& reqID, int event_time);
    void onWrongLink(ItemType fromType, ItemType toType);

signals:
    void saveDialogRequest();

private:
    static const int ItemTypeRole = Qt::UserRole + 1;
    Ui::Document *ui;
    ModelScene *scene;
    ModelStorage *storage;
    QTextEdit *code;
    QMenu *logMenu;
    QStandardItemModel *logModel;
    QAction *startAction, *stopAction;
    bool bSimulating;

    bool tryApplyCode();
    QString timeToString(int time);
};

#endif // DOCUMENT_H
