// основные функции

//      qmodel::model<> *LoadQModel(QString xmlFileName)
// возвращает ссылку на объект типа qmodel::model<>.
// в качестве параметра требует путь или имя файла
// пример qmodel::model<> *newModel = qmodel::converter::LoadQModel(FileName);

// void SaveQModel(qmodel::model<Type> *curModel, QString xmlFileName)
// сохраняет данные о модели в xml файл
// в качестве аргументов принимает указатель на модель, котораяч будет сохранена
// в файл QString xmlFileName
// пример SaveQModel(newModel,"d:\\new.qxml");

// ATTENTION!!!!
// функция сохранения реализована не полностью, не сохраняет связи между
// хендлерами, генераторами и очередями. =)

// функции AddItem и AddLink вспомогательные, используются в LoadQModel

// если найдется возможность испольовать контейнер объектов разных классов
// то можно будет весьма сильно сократить код!

// на момент этой ревизии (16 вроде ) помимо измений в этой файле, можно
// заметить изменения еще в файле mainWindow.cpp и qt.pro
// в файле проекта добавлен ключ xml (см. qt.pro: строка 7)
// в файле формы реализовано нажатие кнопки "открыть"
// (см. mainwindow.cpp: строка 57,58)

// так же , на всякий пожарный прилагаю листинг файла "example.qxml"
/*
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

#ifndef CONVERTER_H
#define CONVERTER_H

#include "model.h"

#include <QtXml/QDomDocument>

namespace qmodel
{

namespace converter
{
    //добавляет генератор, очередь или хендлер в модель
    template <typename Type>
    void AddItem(qmodel::model<Type> *curModel, QString elemType, QString param)
    {
        QMap<QString,int> entries;
        entries["req_generator"]=0;
        entries["queue"]=1;
        entries["handler"]=2;

        request_generator<Type> *newgen;
        queue<Type> *newqueue;
        handler<Type> *newhnd;

        switch (entries[elemType])
        {
            case 0: newgen = new request_generator<Type>(param.toInt());
                    curModel->req_generators.push_back(*newgen);
                    break;
            case 1: newqueue = new queue<Type>();
                    curModel->queues.push_back(*newqueue);
                    break;
            case 2: newhnd = new handler<Type>(param.toInt());
                    curModel->handlers.push_back(*newhnd);
            default:    break;
        }
    }

    //добавляет линк в модель
    //в две функции, потому что для линка нужно больше параметров.
    //сама связь идет по сути по ид.

    template <typename Type>
    void AddLink(qmodel::model<Type> *curModel, std::vector<QString> params)
    {
        QMap<QString,int> entries;
        entries["link_generator_queue"]=0;
        entries["link_queue_handler"]=1;

        link<qmodel::request_generator<>*,qmodel::queue<>*> *link_gen_que;
        link<qmodel::queue<>*,qmodel::handler<>*> *link_que_hnd;

        switch (entries[params[0]])
        {
            case 0:     link_gen_que = new link<qmodel::request_generator<>*,qmodel::queue<>*>
                                                (&curModel->req_generators[params[1].toInt()],
                                                &curModel->queues[params[2].toInt()]);
                        curModel->link_generators_queues.push_back(*link_gen_que);
                        break;
            case 1:     link_que_hnd = new link<qmodel::queue<>*,qmodel::handler<>*>
                                                (&curModel->queues[params[1].toInt()],
                                                &curModel->handlers[params[2].toInt()]);
                        curModel->link_queues_handlers.push_back(*link_que_hnd);
            default:    break;
        }

    }

    template <typename Type=int>
    void SaveQModel(qmodel::model<Type> *curModel, QString xmlFileName)
    {
        std::vector<QString> entries={"req_generators","queues","handlers","links"};
        std::vector<QString> subEntries={"req_generator","queue","handler","link"};

        std::vector<int> numOfElem =
        {
            curModel->req_generators.size(),
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
                        case 0: for (int j=0; j<curModel->req_generators.size(); j++)
                                {
                                    QDomElement tempSubEntry = xmldoc.createElement(subEntries[i]);
                                    if (curModel->req_generators[j].get_generating_period()!=0)
                                    {
                                        QDomElement value = xmldoc.createElement("time");
                                        QDomText text = xmldoc.createTextNode(QString::number(curModel->req_generators[j].get_generating_period()));
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
    template <typename Type=int>
    qmodel::model<Type> *LoadQModel(QString xmlFileName)
    {
        qmodel::model<Type> *loadedModel;
        loadedModel = new qmodel::model<Type>();

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
    }

    //преобразует XML в набор объектов на виджете/сцене
    void XMLStringToScene(QString XMLString);

    //обратно
    void SceneToXMLString();

} //end namespace converter

} //end namespace qmodel

#endif // CONVERTER_H
