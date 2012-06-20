#ifndef VALIDATOR_H
#define VALIDATOR_H

#include <QObject>
#include "common.h"

//! Класс предназначен для проверки модели на валидность.
/**
  *  Реализует паттерн singleton
  */
class Validator : public QObject
{
    Q_OBJECT

private:
    Validator();

    //QMultiMap<ItemType, ItemType> invalidLinks;

public:
    //! Возвращает объект класса Validator
    static Validator& inst();

    //! Проверяет возможно ли связать fromType и toType.
    //! Если данная связь неосуществима, возвращает false и шлёт сигнал wrongLink().
    bool validateLink(ItemType fromType, ItemType toType);

signals:
    //! Вызывается из validateLink() в случае невозможности создать связь
    void wrongLink(ItemType fromType, ItemType toType);
};

#endif // VALIDATOR_H
