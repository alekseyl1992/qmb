#ifndef MODELSTORAGE_H
#define MODELSTORAGE_H

#include "logic/model.h"
#include "utility/common.h"
#include "utility/ifillablemodel.h"

#include <QDebug>
#include <QtXml/QDomDocument>
#include <QFile>
#include <QPoint>

//�����7
//��������� ����������. ������ ��� � ������
const QString ItemNames[] = {"Generator","Queue",
                            "Handler","Terminator"};

class ModelStorage : public QObject
{
    Q_OBJECT

private:
    logic::model* myModel;
    QDomDocument* curDoc;
// ����
    QDomElement root, gens, queues, handlers, terms;

    //������� ��� ���������� ������ � ������ � ������.
    //����� �� � private

    void AddItem(logic::model *curModel, QString elemType, QString param);
    void AddLink(logic::model *curModel, std::vector<QString> params);

    void MoveItem(QDomElement &item, int &id, QPoint &pos);

public:
    //TODO ��������� ���������� � cpp
    ModelStorage(QString name) : myModel(nullptr)
    {
        curDoc = new QDomDocument("qmodel");

        //������������ "�������" xml ����� (������ ������ - ��� xml)
        root = curDoc->createElement(name);
        curDoc->appendChild(root);

        qDebug() << curDoc->toString() << endl;
    }

    QString getCodeString(); //���������� ������ � XML-����� �����
    bool setCodeString(QString code); //������� ������ � �������� //���������� ��� ������ ���������� ����
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

            //���� ������������ �����-���� ������ �� ������ ����� ��������
            //��� ������� ����������
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
                                    text = xmldoc.createTextNode("��� ������ � lhs.id?");
                                    value.appendChild(text);
                                    tempSubEntry.appendChild(value);

                                    value = xmldoc.createElement("queue_id");
                                    text = xmldoc.createTextNode("��� ������ � lhs.id?");
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
                                    text = xmldoc.createTextNode("��� ������ � lhs.id?");
                                    value.appendChild(text);
                                    tempSubEntry.appendChild(value);

                                    value = xmldoc.createElement("handler_id");
                                    text = xmldoc.createTextNode("��� ������ � lhs.id?");
                                    value.appendChild(text);
                                    tempSubEntry.appendChild(value);

                                    tempEntry.appendChild(tempSubEntry);
                                }

                        default: break;
                    }
                }

        //    qDebug() << xmldoc.toString();

            //� ���������� ��� ���������� ������,� ������� ������ ����
            //� ���-�� �������� ������� ��� �������� ������
            xmldoc.save(out,2);

            xmlfile.close();
        }
    }

    // ����������� xml ����, � QDomDocument � ����� � QModel
    model *LoadQModel(QString xmlFileName)
    {
        model *loadedModel;
        loadedModel = new model();

        QFile xmlfile(xmlFileName);
        //��������� ������ �� xml ����� � QDomDocument
        QDomDocument xmldoc;
        if (xmlfile.open(QIODevice::ReadOnly))
        {
            if (xmldoc.setContent(&xmlfile))
            {
                // �������� �����
                QDomElement root = xmldoc.documentElement();
                // ���� xml �� ��� ����� ���������, �������
                if (root.tagName() != "qmodel")
                    return NULL;
                else
                {
                    // ���� ������ (����������, �������, ������)
                    QDomNode entry = root.firstChild();
                    for (int i=0; !entry.isNull(); i++)
                    {
                        // ��������������� ��� ������ ����
                        QDomNode subentry = entry.firstChild();
                        for (int j=0; !subentry.isNull(); j++)
                        {
                            // �������� �������
                            // �����������  0 ��� 1 ��������,
                            // ��� ����� �������� ������ � ������
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
                            //��������� ������
                            subentry = subentry.nextSibling();
                        }
                        //��������� ��� �������
                        entry = entry.nextSibling();
                    }
                }
            }
            xmlfile.close();
        }

        //������������ ������ �� ������
        return loadedModel;
    }*/

    //TODO form model here (instead of using LoadQModel)
    logic::model *getModel(bool create = false);
    void freeModel();

    //��������� ���� name, item'� �� ��� id ��� ����������� �� �����
    QString getItemName(int id); // ? �� ����� //����� ������� � ����������
    bool saveModel(QString path);
    bool loadModel(QString path);
    void fillModel(IFillableModel *iModel) const;

    //TODO ����� ����� ������ ��� ��������� � ������ ������� ������ ����������
    //������ ���� ��� �� �������� ������ �������� ���� ������ ��������

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
    };
}; //class

#endif // MODELSTORAGE_H
