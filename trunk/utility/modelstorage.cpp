#include "modelstorage.h"
#include <QPoint>

ModelStorage::ModelStorage() : myModel(nullptr)
{
    curDoc = new QDomDocument("qmodel");
    //необходимо для правильного чтения кириллицы из xml
    QDomNode DocType = curDoc->createProcessingInstruction("xml", "version=\"1.0\" encoding=\"Windows-1251\"");
    curDoc->appendChild(DocType);
    currentPath = "";
    // пока сюда
    history_pos=0; //то бишь начало списка (хотя он итак пуст)

    //заполняем typeNames
    typeNames.insert(ItemType::Generator, "Generator");
    typeNames.insert(ItemType::Queue, "Queue");
    typeNames.insert(ItemType::Handler, "Handler");
    typeNames.insert(ItemType::Terminator, "Terminator");
    typeNames.insert(ItemType::Collector, "Collector");
    typeNames.insert(ItemType::Separator, "Separator");
    typeNames.insert(ItemType::Link, "Link");
}

ModelStorage::~ModelStorage()
{
    delete myModel;
    myModel = nullptr;
}

logic::model* ModelStorage::getModel(bool create)
{
    if(create) //создание модели
    {
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

            ItemType typeId = typeNames.key(ProcessingItem.nodeName());
            switch(typeId)
            {
                case ItemType::Generator:
                    myModel->add_generator(logic::generator(id,period,num_of_reqs));
                    break;

                case ItemType::Queue:
                    myModel->add_queue(logic::queue(id));
                    break;

                case ItemType::Handler:
                    myModel->add_handler(logic::handler(id,period));
                    break;

                case ItemType::Terminator:
                    myModel->add_terminator(logic::terminator(id,period));
                    break;

                case ItemType::Collector:
                    myModel->add_collector(logic::collector(id));
                    break;

                case ItemType::Separator:
                    myModel->add_separator(logic::separator(id));
                    break;

                case ItemType::Link:
                       /*
                        if (fromType==itemTypeToEngString(ItemType::Generator)
                                && toType==itemTypeToEngString(ItemType::Queue))
                            linkType = LinkType::GeneratorToQueue;

                        if (fromType==itemTypeToEngString(ItemType::Queue)
                                && toType==itemTypeToEngString(ItemType::Handler))
                            linkType = LinkType::QueueToHandler;

                        if (fromType==itemTypeToEngString(ItemType::Handler)
                                && toType==itemTypeToEngString(ItemType::Terminator))
                            linkType = LinkType::HandlerToTerminator;
    */
                    AddLink(myModel,fromID,toID);
                    break;

                default:
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

QString ModelStorage::getModelName() const
{
    if (root.hasAttribute("name"))
        return root.attribute("name");
    else return "<некорректное название>";
}

void ModelStorage::setModelName(const QString &name)
{
    if (root.hasAttribute("name"))
        root.setAttribute("name",name);
}

bool ModelStorage::createModel(const QString &name)
{
    // здесь exception бы какой-нить
    root = curDoc->createElement("model");
    root.setAttribute("name",name);
    curDoc->appendChild(root);
    // запоминаем начальный момент
    history.push_front(root.cloneNode().toElement());

    return true;
}

bool ModelStorage::openModel(const QString& path)
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
    currentPath = path;
    return true;
}

bool ModelStorage::saveModel()
{
    QFile xmlFile(currentPath);
    if(xmlFile.open(QIODevice::WriteOnly))
    {
        QTextStream(&xmlFile) << curDoc->toString(3);
        xmlFile.close();
    }
    else return false;
    return true;
}

bool ModelStorage::saveModelAs(const QString &path)
{
    QFile xmlFile(path);
    if(xmlFile.open(QIODevice::WriteOnly))
    {
        QTextStream(&xmlFile) << curDoc->toString(3);
        xmlFile.close();
    }
    else return false;
    currentPath = path;
    return true;
}

bool ModelStorage::redoModel()
{
    if (history.size()!=0 && history_pos > 0)
    {
        curDoc->removeChild(root);
        --history_pos;
        root = history.at(history_pos).cloneNode().toElement();
        curDoc->appendChild(root);
        return true;
    }
    return false;
}

bool ModelStorage::undoModel()
{
    if (history.size()!=0 && history_pos < history.size()-1)
    {
        curDoc->removeChild(root);
        ++history_pos;
        root = history.at(history_pos).cloneNode().toElement();
        curDoc->appendChild(root);
        return true;
    }
    return false;
}

void ModelStorage::fillModel(IFillableModel *iModel) const
{
    QDomElement ProcessingItem = root.firstChildElement();
    iModel->clear();

    while (!ProcessingItem.isNull())
    {
        int id, fromID, toID;
        //QString fromType, toType;

        ItemType itemType = typeNames.key(ProcessingItem.nodeName());
        QPoint pos;
        pos.setX(ProcessingItem.attribute("x").toInt());
        pos.setY(ProcessingItem.attribute("y").toInt());
        QString name = ProcessingItem.attribute("name");

        switch(itemType)
        {
            case ItemType::Generator:
            case ItemType::Queue:
            case ItemType::Handler:
            case ItemType::Terminator:
            case ItemType::Collector:
            case ItemType::Separator:
                id = ProcessingItem.attribute("id").toInt();
                iModel->addItem(itemType,name,id,pos);
                break;

            case ItemType::Link:
                fromID = ProcessingItem.attribute("fromID").toInt();
                toID = ProcessingItem.attribute("toID").toInt();
                iModel->addLink(fromID, toID);
                break;
            default:
                break;
        }
        ProcessingItem = ProcessingItem.nextSiblingElement();
    }
}

QString ModelStorage::getCodeString() const
{
    return curDoc->toString(3);
}

QString ModelStorage::getCurrentPath() const
{
    return currentPath;
}

bool ModelStorage::setCodeString(QString code)
{
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
void ModelStorage::onItemInserted(ItemType type, int id, QString name, QPoint pos)
{
    QDomElement InsertedItem;
    InsertedItem = curDoc->createElement(typeNames[type]);
    InsertedItem.setAttribute("id", id);
    InsertedItem.setAttribute("name", name);

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
        case ItemType::Collector:
            break; //свойства будут
        case ItemType::Separator:
            break; //свойства будут
        default:
            break;
    };
    InsertedItem.setAttribute("x", pos.x());
    InsertedItem.setAttribute("y", pos.y());
    root.appendChild(InsertedItem);

    if (history_pos!=0)
    {
        for (int i=0; i<history_pos; i++)
             history.removeFirst();
        history_pos=0;
    }
    if (history.size()>MAX_STEPS)
        history.removeLast();
    history.push_front(root.cloneNode().toElement());
}

void ModelStorage::onItemMoved(int id, QPoint pos)
{   
    QDomElement MovedItem = root.firstChildElement();

    while (!MovedItem.isNull())
    {
        if (MovedItem.attribute("id").toInt() == id)
        {
            if ( MovedItem.attribute("x").toInt()!=pos.x() &&
                 MovedItem.attribute("y").toInt()!=pos.y() )
               // если координаты действительно разные, то бекап.
               // сделано из-за того, что событие срабатывает при нажатии на элемент
               // необязательно сдвиге
            {
                if (history_pos!=0)
                {
                    for (int i=0; i<history_pos; i++)
                         history.removeFirst();
                    history_pos=0;
                }
                if (history.size()>MAX_STEPS)
                    history.removeLast();
                history.push_front(root.cloneNode().toElement());

                MovedItem.setAttribute("x",pos.x());
                MovedItem.setAttribute("y",pos.y());
            }
        }
        MovedItem = MovedItem.nextSiblingElement();
    }
}

void ModelStorage::onItemRemoved(int id)
{
    QDomElement RemovedItem = root.firstChildElement();
    QDomElement ExistedLink = root.firstChildElement();

    while (!RemovedItem.isNull())
    {
        if (RemovedItem.attribute("id").toInt() == id)
        {
            while (!ExistedLink.isNull())
            {
                if (ExistedLink.attribute("fromID").toInt() == id ||
                     ExistedLink.attribute("toID").toInt() == id)
                {
                    QDomElement nextSibling = ExistedLink.nextSiblingElement();
                    root.removeChild(ExistedLink);
                    ExistedLink = nextSibling;
                }
                else
                    ExistedLink = ExistedLink.nextSiblingElement();
            }
            root.removeChild(RemovedItem);
        }
        RemovedItem = RemovedItem.nextSiblingElement();
    }

    if (history_pos!=0)
    {
        for (int i=0; i<history_pos; i++)
             history.removeFirst();
        history_pos=0;
    }
    if (history.size()>MAX_STEPS)
        history.removeLast();
    history.push_front(root.cloneNode().toElement());

}

void ModelStorage::onLinkInserted(int idFrom, int idTo)
{
    QDomElement InsertedLink;
    InsertedLink = curDoc->createElement(typeNames[ItemType::Link]);
    InsertedLink.setAttribute("fromID",idFrom);
    InsertedLink.setAttribute("toID",idTo);
    root.appendChild(InsertedLink);

    if (history_pos!=0)
    {
        for (int i=0; i<history_pos; i++)
             history.removeFirst();
        history_pos=0;
    }
    if (history.size()>MAX_STEPS)
        history.removeLast();
    history.push_front(root.cloneNode().toElement());
}

void ModelStorage::onLinkRemoved(int idFrom, int idTo)
{
    QDomElement RemovedLink = root.firstChildElement();
    while (!RemovedLink.isNull())
    {
        if ( RemovedLink.nodeName()==typeNames[ItemType::Link] ) // 4 is Link
            if ( RemovedLink.attribute("fromID").toInt()==idFrom &&
                 RemovedLink.attribute("toID").toInt()==idTo )
                root.removeChild(RemovedLink);
        RemovedLink = RemovedLink.nextSiblingElement();
    }

    if (history_pos!=0)
    {
        for (int i=0; i<history_pos; i++)
             history.removeFirst();
        history_pos=0;
    }
    if (history.size()>MAX_STEPS)
        history.removeLast();
    history.push_front(root.cloneNode().toElement());
}
// end реализация слотов //

void ModelStorage::AddLink(logic::model *curModel, int fromID, int toID)
{
    logic::object* obj1 = curModel->find_object(fromID);
    logic::object* obj2 = curModel->find_object(toID);
    curModel->connect(obj1, obj2);
    /*
    switch(linkType)
    {
        case LinkType::GeneratorToQueue:
        {
            generator* gen = curModel->find_generator(fromID);
            queue* que = curModel->find_queue(toID);
            curModel->connect(gen, que);
            //curModel->add_link_generator_queue(link<generator*,queue*>(gen,que));
            break;
        }

        case LinkType::QueueToHandler:
        {
            queue* que = curModel->find_queue(fromID);
            handler* hnd = curModel->find_handler(toID);
            curModel->connect(que, hnd);
            //curModel->add_link_queue_handler(link<queue*, handler*>(que,hnd));
            break;
        }

        case LinkType::HandlerToTerminator:
        {
            handler* hnd = curModel->find_handler(fromID);
            terminator* ter = curModel->find_terminator(toID);
            curModel->connect(hnd, ter);
            //curModel->add_link_handler_terminator(link<handler*, terminator*>(hnd,ter));
            break;
        }
    }*/
}
