#ifndef CONVERTER_H
#define CONVERTER_H
#include "model.h"

namespace qmodel
{

namespace converter
{
    //преобразует XML-документ представленный в виде строки в QModel
    qmodel::model XMLStringToQModel(QString XMLString)
    {
        //вот этим должен занятся Костя в первую очередь
        //2 функции ниже я пока не очень продумал
        //так как я пока не знаю как буду хранить объекты на сцене
        return qmodel();
    }

    //преобразует XML в набор объектов на виджете/сцене
    void XMLStringToScene(QString XMLString)
    {

    }

    //обратно
    void SceneToXMLString()
    {

    }
} //end namespace converter

} //end namespace qmodel

#endif // CONVERTER_H
