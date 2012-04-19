/* �������� �������

//      qmodel::model<> *LoadQModel(QString xmlFileName)
// ���������� ������ �� ������ ���� qmodel::model<>.
// � �������� ��������� ������� ���� ��� ��� �����
// ������ qmodel::model<> *newModel = qmodel::converter::LoadQModel(FileName);

// void SaveQModel(qmodel::model<Type> *curModel, QString xmlFileName)
// ��������� ������ � ������ � xml ����
// � �������� ���������� ��������� ��������� �� ������, �������� ����� ���������
// � ���� QString xmlFileName
// ������ SaveQModel(newModel,"d:\\new.qxml");

// ATTENTION!!!!
// ������� ���������� ����������� �� ���������, �� ��������� ����� �����
// ����������, ������������ � ���������. =)

// ������� AddItem � AddLink ���������������, ������������ � LoadQModel

// ���� �������� ����������� ����������� ��������� �������� ������ �������
// �� ����� ����� ������ ������ ��������� ���!

// �� ������ ���� ������� (16 ����� ) ������ ������� � ���� �����, �����
// �������� ��������� ��� � ����� mainWindow.cpp � qt.pro
// � ����� ������� �������� ���� xml (��. qt.pro: ������ 7)
// � ����� ����� ����������� ������� ������ "�������"
// (��. mainwindow.cpp: ������ 57,58)

// ��� �� , �� ������ �������� �������� ������� ����� "example.qxml"
*/
/* example.qxml
<!DOCTYPE qmodel>
<qmodel>
 <req_generators>
  <req_generator>
   <time>1000</time>
  </req_generator>
 </req_generators>
 <queues>
  <queue/>
 </queues>
 <handlers>
  <handler>
   <time>3000</time>
  </handler>
 </handlers>
 <links>
  <link>
   <type>link_generator_queue</type>
   <req_generator_id>0</req_generator_id>
   <queue_id>0</queue_id>
  </link>
  <link>
   <type>link_queue_handler</type>
   <queue_id>0</queue_id>
   <handler_id>0</handler_id>
  </link>
 </links>
</qmodel>
*/

#ifndef MODELSTORAGE_H
#define MODELSTORAGE_H

#include "model.h"
#include "../modelscene.h"
#include "../common.h"

#include <QDebug>
#include <QtXml/QDomDocument>

//TODO: �������� �� cpp/h
namespace qmodel
{

class ModelStorage : public QObject //��� connect
{
    Q_OBJECT

public:
    //TODO
    ModelStorage(QString name) {}

    //��������� ���������, ������� ��� ������� � ������
    void AddItem(qmodel::model *curModel, QString elemType, QString param)
    {
        QMap<QString, ItemType> entries;
        entries["req_generator"] = ItemType::Generator;
        entries["queue"] = ItemType::Queue;
        entries["handler"] = ItemType::Handler;
        entries["terminator"] = ItemType::Terminator;

        generator *newgen;
        queue *newqueue;
        handler *newhnd;

        //TODO �������� Terminator
        switch (entries[elemType])
        {
            case ItemType::Generator:
                newgen = new generator(param.toInt());
                curModel->generators.push_back(*newgen);
                break;
            case ItemType::Queue:
                newqueue = new queue();
                curModel->queues.push_back(*newqueue);
                break;
            case ItemType::Handler:
                newhnd = new handler(param.toInt());
                curModel->handlers.push_back(*newhnd);
        }
    }

    //��������� ���� � ������
    //� ��� �������, ������ ��� ��� ����� ����� ������ ����������.
    //���� ����� ���� �� ���� �� ��.

    void AddLink(qmodel::model *curModel, std::vector<QString> params)
    {
        QMap<QString,int> entries;
        //TODO ������� �� �������� � AddItem
        entries["link_generator_queue"]=0;
        entries["link_queue_handler"]=1;

        link<generator*,queue*> *link_gen_que;
        link<queue*,handler*> *link_que_hnd;

        switch (entries[params[0]])
        {
            case 0:     link_gen_que = new link<generator*,queue*>
                                                (&curModel->generators[params[1].toInt()],
                                                &curModel->queues[params[2].toInt()]);
                        curModel->link_generators_queues.push_back(*link_gen_que);
                        break;
            case 1:     link_que_hnd = new link<queue*,handler*>
                                                (&curModel->queues[params[1].toInt()],
                                                &curModel->handlers[params[2].toInt()]);
                        curModel->link_queues_handlers.push_back(*link_que_hnd);
            default:    break;
        }

    }

    //DEPRECATED
    void SaveQModel(model *curModel, QString xmlFileName)
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
    qmodel::model *LoadQModel(QString xmlFileName)
    {
        qmodel::model *loadedModel;
        loadedModel = new qmodel::model();

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
    }

    //����������� XML � ����� �������� �� �����
    void XMLToScene(QString FileName, ModelScene *scene)
    {

    }

    //�������
    //��� ���������� ������ ���� � ����� ������ �����������
    //DEPRECATED!, �� ���� �� �������
    void SceneToXML(ModelScene *scene, QString FileName)
    {
        foreach(QGraphicsItem *it, scene->items())
        {
            ModelItem *element = qgraphicsitem_cast<ModelItem *>(it);
            //����� ���������
            //element->id()
            //element->pos()
            //�����:
            //element->arrows()
        }
    }

public slots:
    //TODO ��������� ������� :D
    void onItemInserted(ItemType type, int id, QPoint pos)
    {

    }

    void onItemMoved(int id, QPoint pos)
    {

    }

    void onItemRemoved(int id)
    {

    }
}; //class

} //namespace qmodel

#endif // MODELSTORAGE_H
