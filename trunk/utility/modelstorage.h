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

//зачем7
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

    enum class LinkType : int
    {
        GeneratorToQueue,
        QueueToHandler,
        HandlerToTerminator,
    };

    //функции для добавления линков и итемов в модель.
    //убрал их в private

    void AddLink(logic::model *curModel, LinkType linkType, int fromID, int toID);

public:
    //TODO перенести реализации в cpp
    ModelStorage(QString name) : myModel(nullptr)
    {
        curDoc = new QDomDocument("qmodel");

        //формирование "подписи" xml файла (первая строка - тип xml)
        //!!! Название модели не должно содержать пробел и кириллицы
        //!!! Иначе мы потом эту модель не загрузим. потому делаем чек.
        name.remove(" ");
        for (char c='А'; c<='я'; c++)
            name.remove(c);
        name="model_"+name;
        //вообще, наверное лучше сделать так, чтобы
        //сразу попадал корректный нейм

        root = curDoc->createElement(name);
        curDoc->appendChild(root);

//        qDebug() << loadModel("D:/work/example.qxml");
    }

    QString getCodeString(); //возвращает строку с XML-кодом сцены
    bool setCodeString(QString code); //парсинг строки в документ //вызывается при ручном изсменении кода

    //TODO form model here (instead of using LoadQModel)
    logic::model *getModel(bool create = false);
    void freeModel();

    //получения поля name, item'а по его id для отображения на сцене
    QString getItemName(int id);
    bool saveModel(QString path);
    bool loadModel(QString path);
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
