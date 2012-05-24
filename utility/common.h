#ifndef COMMON_H
#define COMMON_H

#include <QDebug>
#include <chrono>

//! Перечисление составляющих модели
enum class ItemType : int
{
    Generator,
    Queue,
    Handler,
    Terminator
};

//! Преобразует тип элемента в строковое представление
QString itemTypeToString(ItemType type);

typedef unsigned long long ull_t;

ull_t get_now_time();

#endif // COMMON_H
