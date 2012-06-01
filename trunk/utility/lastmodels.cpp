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
    //��������� ������ �������� ��������
    QFile lastModelsFile(QApplication::applicationDirPath() + fileName);
    lastModelsFile.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream stream(&lastModelsFile);
    if(stream.status() == QTextStream::Ok)
    {
        //��������� ���� �� ������
        QStringList list = stream.readAll()
                .split(separator, QString::SkipEmptyParts);
        bool bListChanged = false;

        for(int i=0; i < list.size(); i++)
        {
            QString path = list.at(i).trimmed();
            if(!QFile::exists(path))
            {
                list.removeAt(i);
                if(path != "") //������ ������ ���������� ������� �� �����
                    bListChanged = true;
            }
        }

        if(bListChanged)
        {
            //�������������� ����-������
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
        return QStringList(); //������ ������
}

void LastModels::add(const QString &path)
{
    QFile lastModelsFile(QApplication::applicationDirPath() + fileName);
    lastModelsFile.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream stream(&lastModelsFile);
    if(stream.status() == QTextStream::Ok)
    {
        //��������� ���� �� ������
        QStringList list = stream.readAll()
                .split(separator, QString::SkipEmptyParts);

        list.insert(list.begin(), path);
        list.removeDuplicates();

        //�������������� ����-������
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

