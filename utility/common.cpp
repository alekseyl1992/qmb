#include "common.h"

QString itemTypeToString(ItemType type)
{
    switch (type)
    {
        case ItemType::Generator:
            return "���������";
        case ItemType::Queue:
            return "�������";
        case ItemType::Handler:
            return "����������";
        case ItemType::Terminator:
            return "����������";

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
