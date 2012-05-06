#include "modelstorage.h"
#include <QPoint>


logic::model* ModelStorage::getModel(bool create)
{
    if(create) //создание модели
    {
        myModel = new logic::model();
        myModel->add_generator(logic::generator(myModel, 1, 100, 8));
        myModel->add_generator(logic::generator(myModel, 2, 700, 5));
        myModel->add_queue(logic::queue(myModel, 1));
        myModel->add_queue(logic::queue(myModel, 2));
        myModel->add_handler(logic::handler(myModel, 1, 500));
        myModel->add_handler(logic::handler(myModel, 2, 800));

        logic::generator* g1 = myModel->get_generator_by_id(1);
        logic::generator* g2 = myModel->get_generator_by_id(2);
        logic::queue* q1 = myModel->get_queue_by_id(1);
        //logic::queue* q2 = myModel->get_queue_by_id(2);
        logic::handler* h1 = myModel->get_handler_by_id(1);
        logic::handler* h2 = myModel->get_handler_by_id(2);

        myModel->add_link_generator_queue(logic::link<logic::generator*, logic::queue* >(g1, q1));
        myModel->add_link_generator_queue(logic::link<logic::generator*, logic::queue* >(g2, q1));

        myModel->add_link_queue_handler(logic::link<logic::queue*, logic::handler* >(q1, h1));
        myModel->add_link_queue_handler(logic::link<logic::queue*, logic::handler* >(q1, h2));
    }

    return myModel;
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
    //находишь в QDomDocument item с соотв. id, возвращаешь его поле name
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

void ModelStorage::MoveItem(QDomElement& item, int& id, QPoint& pos)
{
    while (QString(item.attribute("id")).toInt()!=id)
        item=item.nextSiblingElement();
    item.setAttribute("x",pos.x());
    item.setAttribute("y",pos.y());
}

QString ModelStorage::getCodeString()
{
    return curDoc->toString();
}

bool ModelStorage::setCodeString(QString code)
{
    //при возникновении ошибки:
    //throw ParseException("some text", stringNumber);
    return true;
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

//добавляет генератор, очередь или хендлер в модель
void ModelStorage::AddItem(logic::model *curModel, QString elemType, QString param)
{
    QMap<QString, ItemType> entries;
    entries[ItemNames[0]] = ItemType::Generator;
    entries[ItemNames[1]] = ItemType::Queue;
    entries[ItemNames[2]] = ItemType::Handler;
    entries[ItemNames[3]] = ItemType::Terminator;

    //TODO добавить Terminator
    using namespace logic;
    switch (entries[elemType])
    {
        case ItemType::Generator:
            curModel->add_generator(
                        generator(curModel, param.toInt()));
            break;
        case ItemType::Queue:
            curModel->add_queue(
                        queue(curModel));
            break;
        case ItemType::Handler:
            curModel->add_handler(
                        handler(curModel, param.toInt()));
            break;
        case ItemType::Terminator:
            break;
    }
}

void ModelStorage::AddLink(logic::model *curModel, std::vector<QString> params)
{
    QMap<QString,int> entries;
    //TODO сделать по аналогии с AddItem
    entries["link_generator_queue"]=0;
    entries["link_queue_handler"]=1;


    using namespace logic;
    link<generator*,queue*> *link_gen_que;
    link<queue*,handler*> *link_que_hnd;

    switch (entries[params[0]])
    {
        case 0:     curModel->add_link_generator_queue(link<generator*,queue*>
                                            (curModel->get_generator_by_id(params[1].toInt() + 1),
                                             curModel->get_queue_by_id(params[2].toInt() + 1)));
                    break;
        case 1:     curModel->add_link_queue_handler(link<queue*,handler*>
                                            (curModel->get_queue_by_id(params[1].toInt() + 1),
                                            curModel->get_handler_by_id(params[2].toInt() + 1)));
        default:    break;
    }
}
