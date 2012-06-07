#ifndef COMMON_H
#define COMMON_H

#include <QPointF>
#include <QString>
#include <chrono>
#include <QMessageBox>

//! Используется как заглушка для нереализованного функционала
#define Unimplemented() QMessageBox::critical(this, "Внимание", "Данная функция не реализована")
//! Используется, чтобы сообщить приложению о необходимости перезапуска
#define RestartCode 1000

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

//! Перечисление ошибок модели
enum error_code {
    INPUT_IN_GENERATOR,
    OUTPUT_IN_TERMINATOR,
    NO_GENERATORS,
    NO_TERMINATORS,
    N
};

const char* error_code_str[error_code::N] = {
    "INPUT_IN_GENERATOR",
    "OUTPUT_IN_TERMINATOR",
    "NO_GENERATORS",
    "NO_TERMINATORS"
};

//! Преобразует тип элемента в строковое представление
QString itemTypeToString(ItemType type);

//! Вычисляет расстояние маежду двумя точками
qreal distance(QPointF p1, QPointF p2);

typedef unsigned long long ull_t;

#endif // COMMON_H
