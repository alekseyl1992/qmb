/* На всякий случай "ExampleModel.qm"
<?xml version='1.0' encoding='Windows-1251'?>
<!DOCTYPE qmodel>
<model name="Пример модели">
   <Generator x="-314" y="-130" period="500" num_of_reqs="10" id="1"/>
   <Queue x="-136" y="-130" id="1"/>
   <Handler x="56" y="-127" period="600" id="1"/>
   <Terminator x="247" y="-128" period="0" id="1"/>
   <Link fromID="1" toID="1" from="Generator" to="Queue"/>
   <Link fromID="1" toID="1" from="Queue" to="Handler"/>
   <Link fromID="1" toID="1" from="Handler" to="Terminator"/>
</model>
*/ //////////////////////////////////////////

#ifndef MODELSTORAGE_H
#define MODELSTORAGE_H

#include "../logic/model.h"
#include "common.h"
#include "ifillablemodel.h"

#include <QDebug>
#include <QtXml/QDomDocument>
#include <QFile>
#include <QPoint>
#include <QMap>


//маскимальное количество щагов отката
const int MAX_STEPS=15;

//! Класс - хранилище модели
/*!
 * Хранит текущее состояние модели при работе со Сценой или кодом.
 * Также этот класс отвечает за сохранение и загрузку модели.
 * Класс позволяет получить объект model, который передаётся в logic для последующей симуляции и сбора статистики.
 */
class ModelStorage : public QObject
{
    Q_OBJECT

private:
    logic::model* myModel;
    QDomDocument* curDoc; //!< хранилище структуры xml
    QList<QDomElement> history; //!< хранилище undo
    int history_pos; //!< позиция в истории (для redo)
    QDomElement root; //!< корневой элемент (ех: модель №х )
    QString currentPath;

    QMap<ItemType, QString> typeNames;
    /*enum class LinkType : int
    {
        GeneratorToQueue,
        QueueToHandler,
        HandlerToTerminator,
    };*/

    void AddLink(logic::model *curModel, int fromID, int toID);

public:
    ModelStorage();
    ~ModelStorage();

    QString getCodeString() const; //!< возвращает строку с XML-кодом сцены
    bool setCodeString(QString code); //!< парсинг строки в документ //вызывается при ручном изсменении кода
    QString getCurrentPath() const; //!< возвращает текущий путь к файлу документа

    logic::model *getModel(bool create = false);
    void freeModel(); //!< @deprecated На данный момент используется отложенная очистка: при создании новой модели и в деструкторе

    //! получения поля name, item'а по его id для отображения на сцене
    QString getModelName() const;
    void setModelName(const QString &name);
    bool createModel(const QString &name);
    bool openModel(const QString &path);
    bool saveModel();
    bool saveModelAs(const QString &path);
    bool undoModel(); //!< откат модели на один шаг назад
    bool redoModel(); //!< откат модели на один шаг вперед
    void fillModel(IFillableModel *iModel) const; //!< заполнение абстрактной модели

    //TODO здесь будут метода для получения и записи полного списка параметров
    //просто пока ещё не продуман формат хранения этих самых параметров

public slots:

    void onItemInserted(ItemType type, int id, QString name, QPoint pos);
    void onItemMoved(int id, QPoint pos);
    void onItemRemoved(int id);
    void onLinkInserted(int idFrom, int idTo);
    void onLinkRemoved(int idFrom, int idTo);

public:

    class ParseException
    {
    private:
     QString errorText;
     int strNum;

    public:
     ParseException(QString text, int strNumber)
         : errorText(text), strNum(strNumber) {}

     QString text() const
     { return errorText; }

     int stringNum() const
     { return strNum; }

    }; //class exception

}; //class modelstorage

#endif // MODELSTORAGE_H
