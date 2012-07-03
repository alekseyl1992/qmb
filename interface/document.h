#ifndef DOCUMENT_H
#define DOCUMENT_H

#include "interface/scene/modelscene.h"
#include "interface/elementpropertiesdialog.h"
#include "utility/modelstorage.h"
#include "logic/model.h"
#include "logic/request_id.h"
#include <QDialog>
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
    enum Tab {Scene, Code}; //!< перечисление вкладок

    /*! Инициализирует новый документ.
     * После инициализации нужно вызвать createModel() или openModel().
     */
    explicit Document(QWidget *parent);

    ~Document();

    void showLog(bool show = true); //!< показывает/скрывает Лог симуляции
    void setActiveTab(Tab tab); //!< Меняет текущую вкладку (Сцена или Код)

    void startSimulation();
    void pauseSimulation();
    void restoreSimulation();
    void stopSimulation();

    bool isModified() const;
    void setModified(bool m);

    //! сохранялась ли когда-либо модель
    bool isSavable() const;
public slots:
    void on_logButton_toggled(bool checked); //!< Кнопка открытия/скрытия лога симуляции
    void closeEvent(QCloseEvent *event); //!< Срабатывает при попытке закрыть документ
    void keyPressEvent(QKeyEvent *event);
    void clearLog();

    bool createModel(const QString& name);
    bool openModel(const QString& path);
    bool saveModel();
    bool saveModelAs(const QString& path);

private slots:
    //! срабатывает при начале перетаскивания элемента с палитры на Сцену
    void on_toolsView_pressed(const QModelIndex &index);

    void onStartAction(); //!< Нажатие кнопки Старт на панели инструментов
    void onStopAction();  //!< Нажатие кнопки Стоп на панели инструментов
    void onUndoAction();  //!< Нажатие кнопки Отмена на панели инструментов
    void onRedoAction();  //!< Нажатие кнопки Повтор на панели инструментов

    //! срабатывает при смене текущей вкладки Tabs
    void on_tabWidget_currentChanged(int index);

    void on_simulationLog_customContextMenuRequested(const QPoint &pos);

    //слоты для связи с model
    void onReqGenerated(const logic::request_id& reqID, int event_time);
    void onReqQueued(const int& qID, const logic::request_id& reqID, int event_time);
    void onReqBeganHandling(const int& hID, const logic::request_id& reqID, int event_time);
    void onReqFinishedHandling(const int& hID, const logic::request_id& reqID, int event_time);
    void onReqTerminated(const int& tID, const logic::request_id& reqID, int event_time);

    void onSimulationStarted(int time);
    void onSimulationStopped(int time);
    void onSimulationPaused(int time);
    void onSimulationRestored(int time);
    void onSimulationFinished(int time);
    void onSelectionChanged();

    void onWrongLink(ItemType fromType, ItemType toType); //!< Срабатывает при попытке добавления неподдерживаемого типа связи. @see supportedLinks

signals:
    void beganSavable(); //!< Срабатывает после сохранения моедли через кнопку Сохранить на панели инструментов

private:
    static const int ItemTypeRole = Qt::UserRole + 1;
    Ui::Document *ui;
    ModelScene *scene;
    ModelStorage *storage;
    QTextEdit *code;
    QMenu *logMenu;
    QStandardItemModel *logModel; //!< Данные лога симуляции
    QStandardItemModel *propModel;
    ElementPropertiesDialog *propDialog;
    QAction *startAction, *stopAction;
    bool bSimulating; //!< Имеет значение true, если идёт симуляция
    bool bPaused; //!< Имеет значение true, если симуляции поставлена на паузу
    bool bClosing; //!< Имеет значение true, если окно готово к закрытию

    bool tryApplyCode(); //!< Применение кода с вкладки Код к модели
    QString timeToString(int time); //!< Преобразует время из миллисекунд в формат "мм:сс.мс"
};

#endif // DOCUMENT_H
