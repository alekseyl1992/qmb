#ifndef IFILLABLESCENE_H
#define IFILLABLESCENE_H
#include "common.h"

//интерфейс для заполнения модели из ModelStorage
class IFillableModel
{
public:
    virtual void addItem(ItemType itemType, QString name, int id, QPoint pos = QPoint()) = 0;
    virtual void addLink(int idFrom, int idTo) = 0;
};

#endif // IFILLABLESCENE_H
