#ifndef MODELSTORAGE_H
#define MODELSTORAGE_H

#include "logic/model.h"
#include "utility/common.h"
#include "utility/ifillablemodel.h"

#include <QDebug>
#include <QtXml/QDomDocument>
#include <QFile>
#include <QPoint>
#include <QMap>

//�����7
//��������� ����������. ������ ��� � ������
const QString ItemNames[] = {"Generator","Queue",
                             "Handler","Terminator","Link"};

class ModelStorage : public QObject
{
    Q_OBJECT

private:
    logic::model* myModel;
    QDomDocument* curDoc; //��������� ��������� xml
    QDomElement root; //�������� ������� (��: ������ �� )

    enum class LinkType : int
    {
        GeneratorToQueue,
        QueueToHandler,
        HandlerToTerminator,
    };

    //������� ��� ���������� ������ � ������ � ������.
    //����� �� � private

    void AddLink(logic::model *curModel, LinkType linkType, int fromID, int toID);

public:
    //TODO ��������� ���������� � cpp
    ModelStorage(QString name) : myModel(nullptr)
    {
        curDoc = new QDomDocument("qmodel");

        //������������ "�������" xml ����� (������ ������ - ��� xml)
        //!!! �������� ������ �� ������ ��������� ������ � ���������
        //!!! ����� �� ����� ��� ������ �� ��������. ������ ������ ���.
        name.remove(" ");
        for (char c='�'; c<='�'; c++)
            name.remove(c);
        name="model_"+name;
        //������, �������� ����� ������� ���, �����
        //����� ������� ���������� ����

        root = curDoc->createElement(name);
        curDoc->appendChild(root);

//        qDebug() << loadModel("D:/work/example.qxml");
    }

    QString getCodeString(); //���������� ������ � XML-����� �����
    bool setCodeString(QString code); //������� ������ � �������� //���������� ��� ������ ���������� ����

    //TODO form model here (instead of using LoadQModel)
    logic::model *getModel(bool create = false);
    void freeModel();

    //��������� ���� name, item'� �� ��� id ��� ����������� �� �����
    QString getItemName(int id);
    bool saveModel(QString path);
    bool loadModel(QString path);
    void fillModel(IFillableModel *iModel) const;

    //TODO ����� ����� ������ ��� ��������� � ������ ������� ������ ����������
    //������ ���� ��� �� �������� ������ �������� ���� ������ ��������

public slots:

    void onItemInserted(ItemType type, int id, QPoint pos);
    void onItemMoved(ItemType type, int id, QPoint pos);
    void onItemRemoved(ItemType type, int id);
    void onLinkInserted(ItemType fromType, int idFrom, ItemType toType, int idTo);
    void onLinkRemoved(ItemType fromType, int idFrom, ItemType toType, int idTo);

public:

    class ParseException
    {
    private:
     QString errorText;
     int strNum;

    public:
     ParseException(QString text, int strNumber)
         : errorText(text), strNum(strNumber) {}

     QString text() const
     { return errorText; }

     int stringNum() const
     { return strNum; }

    }; //class exception

}; //class modelstorage

#endif // MODELSTORAGE_H
