#ifndef COMMON_H
#define COMMON_H

#include <QPointF>
#include <QString>
#include <chrono>
#include <string>
#include <QMessageBox>

//! Используется как заглушка для нереализованного функционала
#define Unimplemented() QMessageBox::critical(this, "Внимание", "Данная функция не реализована")
//! Используется, чтобы сообщить приложению о необходимости перезапуска
#define RestartCode 1000

typedef unsigned long long ull_t;

//! Перечисление составляющих модели
enum class ItemType : int
{
    NoType,
    Generator,
    Queue,
    Handler,
    Terminator,
    HubIn,
    HubOut
};



//! Преобразует тип элемента в строковое представление
QString itemTypeToString(ItemType type);
QString itemTypeToEngString(ItemType type);
std::string itemTypeTo_stdString(ItemType type);

//! Вычисляет расстояние маежду двумя точками
qreal distance(QPointF p1, QPointF p2);

ull_t get_now_time();




#endif // COMMON_H
