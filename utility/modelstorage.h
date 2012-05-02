#ifndef MODELSTORAGE_H
#define MODELSTORAGE_H

#include "logic/model.h"
#include "utility/common.h"

#include <QDebug>
#include <QtXml/QDomDocument>
#include <QFile>
#include <QPoint>

namespace logic
{

//константы символьные. выношу как и обещал
const QString ItemNames[] = {"Generator","Queue",
                            "Handler","Terminator"};

class ModelStorage : public QObject //для connect
{
    Q_OBJECT

private:
    model* myModel;
    QDomDocument* curDoc;
// пока
    QDomElement root, gens, queues, handlers, terms;

    //функции для добавления линков и итемов в модель.
    //убрал их в private

    void AddItem(model *curModel, QString elemType, QString param);
    void AddLink(model *curModel, std::vector<QString> params);

    void MoveItem(QDomElement &item, int &id, QPoint &pos);

public:
    ModelStorage(QString name) : myModel(nullptr)
    {
        curDoc = new QDomDocument("qmodel");

        //формирование "подписи" xml файла (первая строка - тип xml)
        root = curDoc->createElement(name);
        curDoc->appendChild(root);

        qDebug() << curDoc->toString() << endl;
    }

    //DEPRECATED
    /*void SaveQModel(model *curModel, QString xmlFileName)
    {
        std::vector<QString> entries={"req_generators","queues","handlers","links"};
        std::vector<QString> subEntries={"req_generator","queue","handler","link"};

        std::vector<int> numOfElem =
        {
            curModel->generators.size(),
            curModel->queues.size(),
            curModel->handlers.size(),
            curModel->link_generators_queues.size()+
            curModel->link_queues_handlers.size()
        };

        QFile xmlfile(xmlFileName);
        if (xmlfile.open(QFile::WriteOnly | QFile::Truncate))
        {
            QTextStream out(&xmlfile);
            QDomDocument xmldoc("qmodel");

            QDomElement root = xmldoc.createElement("qmodel");
            xmldoc.appendChild(root);

            //если использовать какой-нить список из разных типов объектов
            //код заметно уменьшится
            for (int i=0; i<numOfElem.size(); i++)
                if (numOfElem[i]>0)
                {
                    QDomElement tempEntry = xmldoc.createElement(entries[i]);
                    root.appendChild(tempEntry);

                    switch (i)
                    {
                        case 0: for (int j=0; j<curModel->generators.size(); j++)
                                {
                                    QDomElement tempSubEntry = xmldoc.createElement(subEntries[i]);
                                    if (curModel->generators[j].get_generating_period()!=0)
                                    {
                                        QDomElement value = xmldoc.createElement("time");
                                        QDomText text = xmldoc.createTextNode(QString::number(curModel->generators[j].get_generating_period()));
                                        value.appendChild(text);
                                        tempSubEntry.appendChild(value);
                                    }
                                    tempEntry.appendChild(tempSubEntry);
                                }
                                break;
                        case 1: for (int j=0; j<curModel->queues.size(); j++)
                                {
                                    QDomElement tempSubEntry = xmldoc.createElement(subEntries[i]);
                                    tempEntry.appendChild(tempSubEntry);
                                }
                                break;
                        case 2: for (int j=0; j<curModel->handlers.size(); j++)
                                {
                                    QDomElement tempSubEntry = xmldoc.createElement(subEntries[i]);
                                    if (curModel->handlers[j].get_handling_period()!=0)
                                    {
                                        QDomElement value = xmldoc.createElement("time");
                                        QDomText text = xmldoc.createTextNode(QString::number(curModel->handlers[j].get_handling_period()));
                                        value.appendChild(text);
                                        tempSubEntry.appendChild(value);
                                    }
                                    tempEntry.appendChild(tempSubEntry);
                                }
                                break;
                        case 3: for (int j=0; j<curModel->link_generators_queues.size(); j++)
                                {
                                    QDomElement tempSubEntry = xmldoc.createElement(subEntries[i]);

                                    QDomElement value = xmldoc.createElement("type");
                                    QDomText text = xmldoc.createTextNode("link_generator_queue");
                                    value.appendChild(text);
                                    tempSubEntry.appendChild(value);

                                    value = xmldoc.createElement("req_generator_id");
                                    text = xmldoc.createTextNode("где доступ к lhs.id?");
                                    value.appendChild(text);
                                    tempSubEntry.appendChild(value);

                                    value = xmldoc.createElement("queue_id");
                                    text = xmldoc.createTextNode("где доступ к lhs.id?");
                                    value.appendChild(text);
                                    tempSubEntry.appendChild(value);

                                    tempEntry.appendChild(tempSubEntry);
                                }

                                for (int j=0; j<curModel->link_queues_handlers.size(); j++)
                                {
                                    QDomElement tempSubEntry = xmldoc.createElement(subEntries[i]);

                                    QDomElement value = xmldoc.createElement("type");
                                    QDomText text = xmldoc.createTextNode("link_queue_handler");
                                    value.appendChild(text);
                                    tempSubEntry.appendChild(value);

                                    value = xmldoc.createElement("queue_id");
                                    text = xmldoc.createTextNode("где доступ к lhs.id?");
                                    value.appendChild(text);
                                    tempSubEntry.appendChild(value);

                                    value = xmldoc.createElement("handler_id");
                                    text = xmldoc.createTextNode("где доступ к lhs.id?");
                                    value.appendChild(text);
                                    tempSubEntry.appendChild(value);

                                    tempEntry.appendChild(tempSubEntry);
                                }

                        default: break;
                    }
                }

        //    qDebug() << xmldoc.toString();

            //в параметрах имя текстового потока,с которым связан файл
            //и кол-во символов отступа для дочерней записи
            xmldoc.save(out,2);

            xmlfile.close();
        }
    }

    // преобразует xml файл, в QDomDocument и затем в QModel
    model *LoadQModel(QString xmlFileName)
    {
        model *loadedModel;
        loadedModel = new model();

        QFile xmlfile(xmlFileName);
        //считываем данные из xml файла в QDomDocument
        QDomDocument xmldoc;
        if (xmlfile.open(QIODevice::ReadOnly))
        {
            if (xmldoc.setContent(&xmlfile))
            {
                // корневой пункт
                QDomElement root = xmldoc.documentElement();
                // если xml не для нашей программы, выходим
                if (root.tagName() != "qmodel")
                    return NULL;
                else
                {
                    // типы данных (генераторы, очереди, прочее)
                    QDomNode entry = root.firstChild();
                    for (int i=0; !entry.isNull(); i++)
                    {
                        // непосредственно сам объект типа
                        QDomNode subentry = entry.firstChild();
                        for (int j=0; !subentry.isNull(); j++)
                        {
                            // свойства объекта
                            // считывается  0 или 1 свойства,
                            // для линка свойства пишутс в вектор
                            QDomNode values = subentry.firstChild();
                            if (subentry.nodeName()=="link")
                            {
                                std::vector<QString> link_params;
                                while(!values.isNull())
                                {
                                    link_params.push_back(values.toElement().text());
                                    values = values.nextSibling();
                                }
                                AddLink(loadedModel,link_params);
                            }
                            else
                            {
                                if (!values.isNull())
                                    AddItem(loadedModel,subentry.nodeName(),values.toElement().text());
                                else
                                    AddItem(loadedModel,subentry.nodeName(),"");
                            }
                            //следующий объект
                            subentry = subentry.nextSibling();
                        }
                        //следующий тип объекта
                        entry = entry.nextSibling();
                    }
                }
            }
            xmlfile.close();
        }

        //возвращается ссылка на модель
        return loadedModel;
    }*/

    //TODO form model here (instead of using LoadQModel)
    model *getModel(bool create = false);

    //получения поля name, item'а по его id для отображения на сцене
    QString getItemName(int id); // ? не понял

    //TODO здесь будут метода для получения и записи полного списка параметров
    //просто пока ещё не продуман формат хранения этих самывх форматов

public slots:  

    void onItemInserted(ItemType type, int id, QPoint pos);
    void onItemMoved(ItemType type, int id, QPoint pos);
    void onItemRemoved(ItemType type, int id);
    void onLinkInserted(ItemType fromType, int idFrom, ItemType toType, int idTo);
    void onLinkRemoved(ItemType fromType, int idFrom, ItemType toType, int idTo);

}; //class

} //namespace logic

#endif // MODELSTORAGE_H
