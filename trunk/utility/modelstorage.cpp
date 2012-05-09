#include "modelstorage.h"
#include <QPoint>

logic::model* ModelStorage::getModel(bool create)
{  
    const int Link = 4;

    if(create) //создание модели
    {
        QMap<QString, int> entries;
        entries[ItemNames[0]] = int(ItemType::Generator);
        entries[ItemNames[1]] = int(ItemType::Queue);
        entries[ItemNames[2]] = int(ItemType::Handler);
        entries[ItemNames[3]] = int(ItemType::Terminator);
        entries[ItemNames[4]] = Link;

        delete myModel;
        myModel = new logic::model();

        QDomElement ProcessingItem = root.firstChildElement();

        while (!ProcessingItem.isNull())
        {
            int period, id, fromID, toID, num_of_reqs;
            QString fromType, toType;
            period = ProcessingItem.attribute("period").toInt();
            id = ProcessingItem.attribute("id").toInt();
            fromID = ProcessingItem.attribute("fromID").toInt();
            toID = ProcessingItem.attribute("toID").toInt();
            fromType = ProcessingItem.attribute("from");
            toType = ProcessingItem.attribute("to");
            num_of_reqs = ProcessingItem.attribute("num_of_reqs").toInt();

            switch(entries[ProcessingItem.nodeName()])
            {
                case int(ItemType::Generator):
                myModel->add_generator(logic::generator(myModel,id,period,num_of_reqs));
                break;

                case int(ItemType::Queue):
                myModel->add_queue(logic::queue(myModel,id));
                break;

                case int(ItemType::Handler):
                myModel->add_handler(logic::handler(myModel,id,period));
                break;

                case int(ItemType::Terminator):
                myModel->add_terminator(logic::terminator(myModel,id,period));
                break;

                case Link: // 4 is Link
                LinkType linkType;

                if (fromType==ItemNames[int(ItemType::Generator)]
                        && toType==ItemNames[int(ItemType::Queue)])
                    linkType = LinkType::GeneratorToQueue;

                if (fromType==ItemNames[int(ItemType::Queue)]
                        && toType==ItemNames[int(ItemType::Handler)])
                    linkType = LinkType::QueueToHandler;

                if (fromType==ItemNames[int(ItemType::Handler)]
                        && toType==ItemNames[int(ItemType::Terminator)])
                    linkType = LinkType::HandlerToTerminator;

                AddLink(myModel,linkType,fromID,toID);
                break;
            }
            ProcessingItem = ProcessingItem.nextSiblingElement();
        }
    }
    return myModel;
}

void ModelStorage::freeModel()
{
    delete myModel;
    myModel = nullptr;
}

QString ModelStorage::getItemName(int id)
{
    return QString(); //stub
    //находишь в QDomDocument item с соотв. id, возвращаешь его поле name

    // если у каждого элемента будет какое-то собственное уникальное имя
    // то смысл ясен, если нет то ?
}

bool ModelStorage::saveModel(QString path)
{
    QFile xmlFile(path);
    if(xmlFile.open(QIODevice::WriteOnly))
    {
        QTextStream(&xmlFile) << curDoc->toString(3);
        xmlFile.close();
    }
    else return false;
    return true;
}

bool ModelStorage::loadModel(QString path)
{
    QFile xmlFile(path);
    //считываем данные из xml файла в QDomDocument
    if (xmlFile.open(QIODevice::ReadOnly))
    {
        delete curDoc;
        curDoc = new QDomDocument("qmodel");
        if (curDoc->setContent(&xmlFile))
            root = curDoc->firstChildElement();
        else return false;
        xmlFile.close();
    }
    else return false;
    return true;
}

void ModelStorage::fillModel(IFillableModel *iModel) const
{
    const int Link = 4;
    QMap<QString, int> entries;
    entries[ItemNames[0]] = int(ItemType::Generator);
    entries[ItemNames[1]] = int(ItemType::Queue);
    entries[ItemNames[2]] = int(ItemType::Handler);
    entries[ItemNames[3]] = int(ItemType::Terminator);
    entries[ItemNames[4]] = Link;

    QDomElement ProcessingItem = root.firstChildElement();

    while (!ProcessingItem.isNull())
    {
        int id, fromID, toID;
        QString fromType, toType;

        int itemType = entries[ProcessingItem.nodeName()];
        QPoint pos;
        pos.setX(ProcessingItem.attribute("x").toInt());
        pos.setY(ProcessingItem.attribute("y").toInt());

        switch((int)itemType)
        {
            case (int)ItemType::Generator:
            case (int)ItemType::Queue:
            case (int)ItemType::Handler:
            case (int)ItemType::Terminator:
            id = ProcessingItem.attribute("id").toInt();
            iModel->addItem(ItemType(itemType),ItemNames[(int)itemType],id,pos);
            break;

            case Link: // 4 is Link
            fromID = ProcessingItem.attribute("fromID").toInt();
            toID = ProcessingItem.attribute("toID").toInt();
            fromType = ProcessingItem.attribute("from");
            toType = ProcessingItem.attribute("to");
            iModel->addLink(ItemType(entries[fromType]),fromID,ItemType(entries[toType]),toID);
            break;
        }
        ProcessingItem = ProcessingItem.nextSiblingElement();
    }
}

QString ModelStorage::getCodeString()
{
    return curDoc->toString(3);
}

bool ModelStorage::setCodeString(QString code)
{
//    qDebug() << "entering the setCodeString!";
    QDomDocument tempDoc;
    if (tempDoc.setContent(code))
    {
        delete curDoc;
        curDoc = new QDomDocument("qmodel");
        curDoc->setContent(code);
        root = curDoc->firstChildElement();
        return true;
    }
    else
    {
        qDebug() << "SetCodeString::Error!";
        return false;
    }
    //при возникновении ошибки:
    //throw ParseException("some text", stringNumber);

    //QDom технология считывает и строит все дерево сразу
    //QSax обрабатывает текст построчно... поэтому реализация исключений
    //с возвратом номера строки я думаю невозможна. нужно что-нибудь другое.
}

// реализация слотов //
void ModelStorage::onItemInserted(ItemType type, int id, QPoint pos)
{
    QDomElement InsertedItem;
    InsertedItem = curDoc->createElement(ItemNames[(int)type]);
    InsertedItem.setAttribute("id",id);
    switch (type)
    {
        case ItemType::Generator:
            InsertedItem.setAttribute("period",500);
            InsertedItem.setAttribute("num_of_reqs",10);
            break;
        case ItemType::Queue:
            break;
        case ItemType::Handler:
            InsertedItem.setAttribute("period",600);
            break;
        case ItemType::Terminator:
            InsertedItem.setAttribute("period",0);
            break;
    };
    InsertedItem.setAttribute("x",pos.x());
    InsertedItem.setAttribute("y",pos.y());
    root.appendChild(InsertedItem);
}

void ModelStorage::onItemMoved(ItemType type, int id, QPoint pos)
{
    QDomElement MovedItem = root.firstChildElement();

    while (!MovedItem.isNull())
    {
        if ( (MovedItem.nodeName()==ItemNames[(int)type]) &&
        (MovedItem.attribute("id").toInt()==id) )
        {
            MovedItem.setAttribute("x",pos.x());
            MovedItem.setAttribute("y",pos.y());
        }
        MovedItem = MovedItem.nextSiblingElement();
    }
}

void ModelStorage::onItemRemoved(ItemType type, int id)
{
    QDomElement RemovedItem = root.firstChildElement();
    QDomElement ExistedLink = root.firstChildElement();

    while (!RemovedItem.isNull())
    {
        if ( (RemovedItem.nodeName()==ItemNames[(int)type]) &&
        (RemovedItem.attribute("id").toInt()==id) )
        {
            while (!ExistedLink.isNull())
            {
                if ( ( ExistedLink.attribute("from")==ItemNames[int(type)] &&
                     ExistedLink.attribute("fromID").toInt()==id ) ||
                     ( ExistedLink.attribute("to")==ItemNames[int(type)] &&
                     ExistedLink.attribute("toID").toInt()==id ) )
                    root.removeChild(ExistedLink);
                ExistedLink = ExistedLink.nextSiblingElement();
            }
            root.removeChild(RemovedItem);
        }
        RemovedItem = RemovedItem.nextSiblingElement();
    }
}

void ModelStorage::onLinkInserted(ItemType fromType, int idFrom, ItemType toType, int idTo)
{
    QDomElement InsertedLink;
    InsertedLink = curDoc->createElement(ItemNames[4]); // 4 is Link
    InsertedLink.setAttribute("from",ItemNames[int(fromType)]);
    InsertedLink.setAttribute("fromID",idFrom);
    InsertedLink.setAttribute("to",ItemNames[int(toType)]);
    InsertedLink.setAttribute("toID",idTo);
    root.appendChild(InsertedLink);
}

void ModelStorage::onLinkRemoved(ItemType fromType, int idFrom, ItemType toType, int idTo)
{
    QDomElement RemovedLink = root.firstChildElement();
    while (!RemovedLink.isNull())
    {
        if ( RemovedLink.nodeName()==ItemNames[4] ) // 4 is Link
            if ( RemovedLink.attribute("from")==ItemNames[int(fromType)] &&
                 RemovedLink.attribute("fromID").toInt()==idFrom &&
                 RemovedLink.attribute("to")==ItemNames[int(toType)] &&
                 RemovedLink.attribute("toID").toInt()==idTo )
                root.removeChild(RemovedLink);
        RemovedLink = RemovedLink.nextSiblingElement();
    }
}
// end реализация слотов //

void ModelStorage::AddLink(logic::model *curModel, LinkType linkType, int fromID, int toID)
{
    using namespace logic;
    switch(linkType)
    {
        case LinkType::GeneratorToQueue:
        {
            generator* gen = curModel->get_generator_by_id(fromID);
            queue* que = curModel->get_queue_by_id(toID);
            curModel->add_link_generator_queue(link<generator*,queue*>(gen,que));
            break;
        }

        case LinkType::QueueToHandler:
        {
            queue* que = curModel->get_queue_by_id(fromID);
            handler* hnd = curModel->get_handler_by_id(toID);
            curModel->add_link_queue_handler(link<queue*, handler*>(que,hnd));
            break;
        }

        case LinkType::HandlerToTerminator:
        {
            handler* hnd = curModel->get_handler_by_id(fromID);
            terminator* ter = curModel->get_terminator_by_id(toID);
            curModel->add_link_handler_terminator(link<handler*, terminator*>(hnd,ter));
            break;
        }
    }
}
