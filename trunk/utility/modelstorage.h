/* �� ������ ������ "ExampleModel.qm"
<?xml version='1.0' encoding='Windows-1251'?>
<!DOCTYPE qmodel>
<model name="������ ������">
   <Generator x="-314" y="-130" period="500" num_of_reqs="10" id="1"/>
   <Queue x="-136" y="-130" id="1"/>
   <Handler x="56" y="-127" period="600" id="1"/>
   <Terminator x="247" y="-128" period="0" id="1"/>
   <Link fromID="1" toID="1" from="Generator" to="Queue"/>
   <Link fromID="1" toID="1" from="Queue" to="Handler"/>
   <Link fromID="1" toID="1" from="Handler" to="Terminator"/>
</model>
*/ //////////////////////////////////////////

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
    QString currentPath;

    enum class LinkType : int
    {
        GeneratorToQueue,
        QueueToHandler,
        HandlerToTerminator,
    };

    void AddLink(logic::model *curModel, LinkType linkType, int fromID, int toID);

public:
    ModelStorage();

    QString getCodeString() const; //���������� ������ � XML-����� �����
    bool setCodeString(QString code); //������� ������ � �������� //���������� ��� ������ ���������� ����
    QString getCurrentPath() const; //���������� ������� ���� � ����� ���������

    logic::model *getModel(bool create = false);
    void freeModel();

    //��������� ���� name, item'� �� ��� id ��� ����������� �� �����
    QString getModelName() const;
    void setModelName(const QString &name);
    bool createModel(const QString &name);
    bool openModel(const QString &path);
    bool saveModel();
    bool saveModelAs(const QString &path);
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
