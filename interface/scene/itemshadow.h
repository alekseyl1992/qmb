#ifndef ITEMSHADOW_H
#define ITEMSHADOW_H
#include <QGraphicsDropShadowEffect>
#include <QGraphicsItem>

//! Если наследоваться от этого класса, то у наследника появиться тень
class ItemShadow
{
public:
    ItemShadow(QGraphicsItem *item, bool drop);
    ~ItemShadow();

    virtual void scaleShadow(qreal factor);

private:
    QGraphicsDropShadowEffect *effect;
    const qreal offset;
    const qreal blurRadius;
};

#endif // ITEMSHADOW_H
