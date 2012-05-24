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

ull_t get_now_time()
{
    auto now = std::chrono::system_clock::now();
    auto duration  = now.time_since_epoch();
    return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
}
