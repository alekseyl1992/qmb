#ifndef COMMON_H
#define COMMON_H

#include <QDebug>

enum class ItemType : int
{
    Generator,
    Queue,
    Handler,
    Terminator
};

typedef unsigned long long ull_t;

#endif // COMMON_H
