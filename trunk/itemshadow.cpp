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
    offset *= factor;
    blurRadius *= factor;
    effect->setOffset(offset);
    effect->setBlurRadius(blurRadius);
}
