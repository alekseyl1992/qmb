#include "modelstorage.h"
#include <QPoint>

namespace logic
{
    model* ModelStorage::getModel(bool create)
    {
        if(create) //�������� ������
        {
            myModel = new model();
            myModel->generators.push_back(logic::generator(myModel, 1, 100, 8));
            myModel->generators.push_back(logic::generator(myModel, 2, 700, 5));
            myModel->queues.push_back(logic::queue(myModel, 1));
            myModel->queues.push_back(logic::queue(myModel, 2));
            myModel->handlers.push_back(logic::handler(myModel, 1, 500));
            myModel->handlers.push_back(logic::handler(myModel, 2, 800));

            myModel->link_generators_queues.push_back(logic::link<logic::generator*, logic::queue* >(&myModel->generators[0], &myModel->queues[0]));
            myModel->link_generators_queues.push_back(logic::link<logic::generator*, logic::queue* >(&myModel->generators[1], &myModel->queues[1]));

            myModel->link_queues_handlers.push_back(logic::link<logic::queue*, logic::handler* >(&myModel->queues[0], &myModel->handlers[0]));
            myModel->link_queues_handlers.push_back(logic::link<logic::queue*, logic::handler* >(&myModel->queues[1], &myModel->handlers[0]));
        }

        return myModel;
    }

/*    void ModelStorage::SceneToXML(ModelScene *scene, QString FileName)
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
    }*/

    QString ModelStorage::getItemName(int id)
    {
        return QString(); //stub
    }

    bool ModelStorage::saveModel(QString path)
    {
        return true;
    }

    bool ModelStorage::loadModel(QString path)
    {
        return true;
    }

    void ModelStorage::fillModel(IFillableModel *iModel)
    {

    }


    void ModelStorage::onItemInserted(ItemType type, int id, QPoint pos)
    {
        switch (type)
        {
            case ItemType::Generator:
            {
                //���� ��������� ������� ���� �� ���� �� ������� �������
                //������� ������ ��������� ������� ����
                if (gens.parentNode()==curDoc->toElement())
                    gens = curDoc->createElement(ItemNames[(int)ItemType::Generator]+"s");

                QDomElement newgen;
                newgen = curDoc->createElement(ItemNames[(int)ItemType::Generator]);
                newgen.setAttribute("id",id);
                newgen.setAttribute("time",500);
                newgen.setAttribute("x",pos.x());
                newgen.setAttribute("y",pos.y());
                gens.appendChild(newgen);
                root.appendChild(gens);
                qDebug() << curDoc->toString() << endl;
                break;
            }
            case ItemType::Queue:
            {
                if (queues.parentNode()==curDoc->toElement())
                    queues = curDoc->createElement(ItemNames[(int)ItemType::Queue]+"s");
                QDomElement newqueue;
                newqueue = curDoc->createElement(ItemNames[(int)ItemType::Queue]);
                newqueue.setAttribute("id",id);
                newqueue.setAttribute("x",pos.x());
                newqueue.setAttribute("y",pos.y());
                queues.appendChild(newqueue);
                root.appendChild(queues);
                qDebug() << curDoc->toString() << endl;
                break;
            }
            case ItemType::Handler:
            {
                if (handlers.parentNode()==curDoc->toElement())
                    handlers = curDoc->createElement(ItemNames[(int)ItemType::Handler]+"s");
                QDomElement newhandler;
                newhandler = curDoc->createElement(ItemNames[(int)ItemType::Handler]);
                newhandler.setAttribute("id",id);
                newhandler.setAttribute("time",500);
                newhandler.setAttribute("x",pos.x());
                newhandler.setAttribute("y",pos.y());
                handlers.appendChild(newhandler);
                root.appendChild(handlers);
                qDebug() << curDoc->toString() << endl;
                break;
            }
            case ItemType::Terminator:
            {
                if (terms.parentNode()==curDoc->toElement())
                    terms = curDoc->createElement(ItemNames[(int)ItemType::Terminator]+"s");
                QDomElement newterm;
                newterm = curDoc->createElement(ItemNames[(int)ItemType::Terminator]);
                newterm.setAttribute("id",id);
                newterm.setAttribute("x",pos.x());
                newterm.setAttribute("y",pos.y());
                terms.appendChild(newterm);
                root.appendChild(terms);
                qDebug() << curDoc->toString() << endl;
                break;
            }
        };
    }

    void ModelStorage::MoveItem(QDomElement& item, int& id, QPoint& pos)
    {
        while (QString(item.attribute("id")).toInt()!=id)
            item=item.nextSiblingElement();
        item.setAttribute("x",pos.x());
        item.setAttribute("y",pos.y());
    }

    void ModelStorage::onItemMoved(ItemType type, int id, QPoint pos)
    {
        QDomElement elemtomove;
        switch (type)
        {
            case ItemType::Generator:
                elemtomove=gens.firstChildElement();
                break;
            case ItemType::Queue:
                elemtomove=queues.firstChildElement();
                break;
            case ItemType::Handler:
                elemtomove=handlers.firstChildElement();
                break;
            case ItemType::Terminator:
                elemtomove=terms.firstChildElement();
                break;
        };
        MoveItem(elemtomove,id,pos);
        qDebug() << "moved\n" << curDoc->toString();
    }

    void ModelStorage::onItemRemoved(ItemType type, int id)
    {
        qDebug() << "onItemRemoved";
    }

    void ModelStorage::onLinkInserted(ItemType fromType, int idFrom, ItemType toType, int idTo)
    {
        qDebug() << "onLinkInserted";
    }

    void ModelStorage::onLinkRemoved(ItemType fromType, int idFrom, ItemType toType, int idTo)
    {
        qDebug() << "onLinkRemoved";
    }

    //��������� ���������, ������� ��� ������� � ������
    void ModelStorage::AddItem(model *curModel, QString elemType, QString param)
    {
        QMap<QString, ItemType> entries;
        entries[ItemNames[0]] = ItemType::Generator;
        entries[ItemNames[1]] = ItemType::Queue;
        entries[ItemNames[2]] = ItemType::Handler;
        entries[ItemNames[3]] = ItemType::Terminator;

        generator *newgen;
        queue *newqueue;
        handler *newhnd;

        //TODO �������� Terminator
        switch (entries[elemType])
        {
            case ItemType::Generator:
                newgen = new generator(curModel, param.toInt());
                curModel->generators.push_back(*newgen);
                break;
            case ItemType::Queue:
                newqueue = new queue(curModel);
                curModel->queues.push_back(*newqueue);
                break;
            case ItemType::Handler:
                newhnd = new handler(curModel, param.toInt());
                curModel->handlers.push_back(*newhnd);
                break;
            case ItemType::Terminator:
                break;
        }
    }

    void ModelStorage::AddLink(model *curModel, std::vector<QString> params)
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
} //end namespace logic