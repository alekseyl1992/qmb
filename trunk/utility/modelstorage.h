/* На всякий случай "ExampleModel.qm"
<!DOCTYPE qmodel>
<model name="ExampleModel">
   <Generator x="-334" y="-89" period="500" num_of_reqs="10" id="1"/>
   <Queue x="-144" y="-89" id="1"/>
   <Handler x="54" y="-89" period="600" id="1"/>
   <Terminator x="253" y="-87" period="0" id="1"/>
   <Link fromID="1" toID="1" from="Generator" to="Queue"/>
   <Link fromID="1" toID="1" from="Queue" to="Handler"/>
   <Link fromID="1" toID="1" from="Handler" to="Terminator"/>
</model>
*/ //////////////////////////////////////////

#ifndef MODELSTORAGE_H
#define MODELSTORAGE_H

#include "logic/model.h"
#include "utility/common.h"
#include "utility/ifillablemodel.h"

#include <QDebug>
#include <QtXml/QDomDocument>
#include <QFile>
#include <QPoint>
#include <QMap>

//константы символьные. выношу как и обещал
const QString ItemNames[] = {"Generator","Queue",
                             "Handler","Terminator","Link"};

class ModelStorage : public QObject
{
    Q_OBJECT

private:
    logic::model* myModel;
    QDomDocument* curDoc; //хранилище структуры xml
    QDomElement root; //корневой элемент (ех: модель №х )

    QString currentPath;

    enum class LinkType : int
    {
        GeneratorToQueue,
        QueueToHandler,
        HandlerToTerminator,
    };

    void AddLink(logic::model *curModel, LinkType linkType, int fromID, int toID);

public:
    //TODO перенести реализации в cpp
    ModelStorage() : myModel(nullptr)
    {
        curDoc = new QDomDocument("qmodel");

        //TODO всё это придётся перенести в load/create
        //формирование "подписи" xml файла (первая строка - тип xml)
        //!!! Название модели не должно содержать пробел и кириллицы
        //!!! Иначе мы потом эту модель не загрузим. потому делаем чек.
        /*name.remove(" ");
        for (char c='А'; c<='я'; c++)
            name.remove(c);

        //вообще, наверное лучше сделать так, чтобы
        //сразу попадал корректный нейм

        root = curDoc->createElement("model");
        root.setAttribute("name",name);

        curDoc->appendChild(root);

        //как базовое имя //просьба оставить его пустой строкой
        //это будет использоваться как признак того, что модель ещё ниразу не сохранялась
        */
        /*
        можно вместо этого конечно завести флаг типа bSaved,
        но его имя весьма неочевидно, речь идёт не о том сохранён ли,
        а о том - сохранялся ли вообще
        */
        currentPath = "";
    }

    QString getCodeString(); //возвращает строку с XML-кодом сцены
    bool setCodeString(QString code); //парсинг строки в документ //вызывается при ручном изсменении кода
    QString getCurrentPath(); //возвращает текущий путь к файлу документа

    //TODO form model here (instead of using LoadQModel)
    logic::model *getModel(bool create = false);
    void freeModel();

    //получения поля name, item'а по его id для отображения на сцене
    QString getModelName() const;
    void setModelName(const QString &name);
    bool createModel(const QString &name);
    bool openModel(const QString &path);
    bool saveModel();
    bool saveModelAs(const QString &path);
    void fillModel(IFillableModel *iModel) const;

    //TODO здесь будут метода для получения и записи полного списка параметров
    //просто пока ещё не продуман формат хранения этих самывх форматов

public slots:

    void onItemInserted(ItemType type, int id, QPoint pos);
    void onItemMoved(ItemType type, int id, QPoint pos);
    void onItemRemoved(ItemType type, int id);
    void onLinkInserted(ItemType fromType, int idFrom, ItemType toType, int idTo);
    void onLinkRemoved(ItemType fromType, int idFrom, ItemType toType, int idTo);

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
