#include "common.h"

QString itemTypeToString(ItemType type)
{
    switch (type)
    {
        case ItemType::Generator:
            return "Генератор";
        case ItemType::Queue:
            return "Очередь";
        case ItemType::Handler:
            return "Обработчик";
        case ItemType::Terminator:
            return "Терминатор";

    default:
        return "IllegalItemType";
    };
}

qreal distance(QPointF p1, QPointF p2)
{
    return std::sqrt(pow(p1.x()-p2.x(), 2) +
                     pow(p1.y()-p2.y(), 2));
}


