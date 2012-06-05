#include "itemshadow.h"

ItemShadow::ItemShadow(QGraphicsItem *item, bool drop)
    : offset(5), blurRadius(10)
{
    if(drop)
    {
        effect = new QGraphicsDropShadowEffect();
        effect->setBlurRadius(blurRadius);
        effect->setOffset(offset);
        item->setGraphicsEffect(effect);
    }
    else
        effect = nullptr;
}

ItemShadow::~ItemShadow()
{
    delete effect;
}

void ItemShadow::scaleShadow(qreal factor)
{
    if(effect)
    {
        effect->setOffset(offset*factor);
        effect->setBlurRadius(blurRadius*factor);
    }
}
