#ifndef COMMON_H
#define COMMON_H

#include <QPointF>
#include <QString>
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

//! Вычисляет расстояние маежду двумя точками
qreal distance(QPointF p1, QPointF p2);

typedef unsigned long long ull_t;

ull_t get_now_time();

#endif // COMMON_H
