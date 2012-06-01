#include "lastmodels.h"
#include <QFile>
#include <QTextStream>
#include <QApplication>

LastModels::LastModels()
    : fileName("/lastModels.dat"), separator('\n')
{
}

QStringList LastModels::getList()
{
    //формируем список недавних проектов
    QFile lastModelsFile(QApplication::applicationDirPath() + fileName);
    lastModelsFile.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream stream(&lastModelsFile);
    if(stream.status() == QTextStream::Ok)
    {
        //разбиваем файл на строки
        QStringList list = stream.readAll()
                .split(separator, QString::SkipEmptyParts);
        bool bListChanged = false;

        for(int i=0; i < list.size(); i++)
        {
            QString path = list.at(i).trimmed();
            if(!QFile::exists(path))
            {
                list.removeAt(i);
                if(path != "") //пустую строку бесполезно стирать из файла
                    bListChanged = true;
            }
        }

        if(bListChanged)
        {
            //перезаписываем файл-список
            lastModelsFile.close();
            lastModelsFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text);
            QTextStream wstream(&lastModelsFile);
            foreach(QString str, list)
                wstream << str << separator;

            lastModelsFile.close();
        }
        return list;
    }
    else
        return QStringList(); //пустой список
}

void LastModels::add(const QString &path)
{
    QFile lastModelsFile(QApplication::applicationDirPath() + fileName);
    lastModelsFile.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream stream(&lastModelsFile);
    if(stream.status() == QTextStream::Ok)
    {
        //разбиваем файл на строки
        QStringList list = stream.readAll()
                .split(separator, QString::SkipEmptyParts);

        list.insert(list.begin(), path);
        list.removeDuplicates();

        //перезаписываем файл-список
        lastModelsFile.close();
        lastModelsFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text);
        QTextStream wstream(&lastModelsFile);

        for(int i = 0; i < list.size() && i < maxCount; i++)
            wstream << list.at(i) << separator;
        lastModelsFile.close();

        emit changed();
    }
}

LastModels &LastModels::getInst()
{
    static LastModels models;
    return models;
}

