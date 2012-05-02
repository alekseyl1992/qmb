#ifndef ITEMSHADOW_H
#define ITEMSHADOW_H
#include <QGraphicsDropShadowEffect>
#include <QGraphicsItem>

class ItemShadow
{
public:
    ItemShadow(QGraphicsItem *item);
    ~ItemShadow();

    virtual void scaleShadow(qreal factor);

private:
    QGraphicsDropShadowEffect *effect;
    const qreal offset;
    const qreal blurRadius;
};

#endif // ITEMSHADOW_H
