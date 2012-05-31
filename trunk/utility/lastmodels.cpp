#include "lastmodels.h"
#include <QFile>
#include <QTextStream>
#include <QApplication>

LastModels::LastModels()
    : fileName("/lastOpened.dat")
{
}

QStringList LastModels::getList()
{
    //формируем список недавних проектов
    QFile lastModelsFile(QApplication::applicationDirPath() + fileName);
    lastModelsFile.open(QIODevice::ReadOnly);
    QTextStream stream(&lastModelsFile);
    if(stream.status() == QTextStream::Ok)
    {
        QStringList list(stream.readAll()); //разбиваем файл на строки
        bool bListChanged = false;

        for(int i=0; i < list.size(); i++)
        {
            QString path = list.at(i);
            if(!QFile::exists(path))
            {
                list.removeAt(i);
                bListChanged = true;
            }
        }

        if(bListChanged)
        {
            //перезаписываем файл-список
            lastModelsFile.close();
            lastModelsFile.open(QIODevice::WriteOnly | QIODevice::Truncate);
            QTextStream wstream(&lastModelsFile);
            foreach(QString str, list)
                wstream << str;
        }
        return list;
    }
    else
        return QStringList(); //пустой список
}

void LastModels::add(const QString &path)
{
    QFile lastModelsFile(QApplication::applicationDirPath() + fileName);
    lastModelsFile.open(QIODevice::ReadOnly);
    QTextStream stream(&lastModelsFile);
    if(stream.status() == QTextStream::Ok)
    {
        QStringList list(stream.readAll()); //разбиваем файл на строки
        list.insert(list.begin(), path);
        list.removeDuplicates();

        //перезаписываем файл-список
        lastModelsFile.close();
        lastModelsFile.open(QIODevice::WriteOnly | QIODevice::Truncate);
        QTextStream wstream(&lastModelsFile);
        for(int i = 0; i < maxCount; i++)
            wstream << list.at(i);
    }
}

LastModels &LastModels::getInst()
{
    static LastModels models;
    return models;
}

