#ifndef COMMON_H
#define COMMON_H

#include <QDebug>
#include <chrono>

/*!
 * ѕеречисление составл€ющий модели
 */
enum class ItemType : int
{
    Generator,
    Queue,
    Handler,
    Terminator
};

typedef unsigned long long ull_t;

ull_t get_now_time();

#endif // COMMON_H
