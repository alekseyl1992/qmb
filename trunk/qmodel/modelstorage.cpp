#include "modelstorage.h"

namespace qmodel
{
    model* ModelStorage::getModel(bool create)
    {
        if(create) //создание модели
        {
            myModel = new model();
            myModel->generators.push_back(qmodel::generator(500, 10));
            myModel->generators.push_back(qmodel::generator(700, 5));
            myModel->queues.push_back(qmodel::queue());
            myModel->queues.push_back(qmodel::queue());
            myModel->handlers.push_back(qmodel::handler(600));
            myModel->handlers.push_back(qmodel::handler(800));

            myModel->link_generators_queues.push_back(qmodel::link<qmodel::generator*, qmodel::queue* >(&myModel->generators[0], &myModel->queues[0]));
           // myModel->link_generators_queues.push_back(qmodel::link<qmodel::generator*, qmodel::queue* >(&myModel->generators[1], &myModel->queues[0]));

            myModel->link_queues_handlers.push_back(qmodel::link<qmodel::queue*, qmodel::handler* >(&myModel->queues[0], &myModel->handlers[0]));
            //myModel->link_queues_handlers.push_back(qmodel::link<qmodel::queue*, qmodel::handler* >(&myModel->queues[0], &myModel->handlers[1]));
        }

        return myModel;
    };

    void ModelStorage::XMLToScene(QString FileName, ModelScene *scene)
    {

    }

/*    void ModelStorage::SceneToXML(ModelScene *scene, QString FileName)
    {
        foreach(QGraphicsItem *it, scene->items())
        {
            ModelItem *element = qgraphicsitem_cast<ModelItem *>(it);
            //нужно сохранять
            //element->id()
            //element->pos()
            //связи:
            //element->arrows()
        }
    }*/

    QString ModelStorage::getItemName(int id)
    {
        return QString(); //stub
    }

    void ModelStorage::onItemInserted(ItemType type, int id, QPoint pos)
    {
        switch (type)
        {
        case ItemType::Generator:
        {
            //если элементов данного типо не было до данного момента
            //создаем раздел элементов данного типа
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

    void ModelStorage::onItemMoved(ItemType type, int id, QPoint pos)
    {
        qDebug() << "onItemMoved";
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

    //добавляет генератор, очередь или хендлер в модель
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

        //TODO добавить Terminator
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

    void ModelStorage::AddLink(model *curModel, std::vector<QString> params)
    {
        QMap<QString,int> entries;
        //TODO сделать по аналогии с AddItem
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
}
