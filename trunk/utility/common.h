#ifndef COMMON_H
#define COMMON_H

#include <QPointF>
#include <QString>
#include <QSet>
#include <chrono>
#include <string>
#include <QMessageBox>

//! Используется как заглушка для нереализованного функционала
void Unimplemented(QWidget *w = nullptr);
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
    Collector,
    Separator,
    Link
};


//! Преобразует тип элемента в строковое представление для GUI
QString itemTypeToGUIString(ItemType type);
//! Преобразует тип элемента в строковое представление для XML
QString itemTypeToXMLString(ItemType type);

//! Вычисляет расстояние маежду двумя точками
qreal distance(QPointF p1, QPointF p2);

unsigned long long get_now_time();

//! Находит свободный id в списке
int getFreeId(QSet<int> ids);


#endif // COMMON_H
