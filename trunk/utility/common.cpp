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

ull_t get_now_time()
{
    auto now = std::chrono::system_clock::now();
    auto duration  = now.time_since_epoch();
    return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
}
