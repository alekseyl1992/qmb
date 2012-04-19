#include "itemshadow.h"

ItemShadow::ItemShadow(QGraphicsItem *item)
    : blurRadius(10), offset(5)
{
    effect = new QGraphicsDropShadowEffect();
    effect->setBlurRadius(blurRadius);
    effect->setOffset(offset);
    item->setGraphicsEffect(effect);
}

ItemShadow::~ItemShadow()
{
    delete effect;
}

void ItemShadow::scaleShadow(qreal factor)
{
    effect->setOffset(offset*factor);
    effect->setBlurRadius(blurRadius*factor);
}
