#ifndef IFILLABLESCENE_H
#define IFILLABLESCENE_H
#include "utility/common.h"
#include <QPoint>

//��������� ��� ���������� ������ �� ModelStorage
class IFillableModel
{
public:
    virtual void addItem(ItemType itemType, QString name, int id, QPoint pos = QPoint()) = 0;
    virtual void addLink(ItemType fromType, int idFrom, ItemType toType, int idTo) = 0;
};

#endif // IFILLABLESCENE_H
