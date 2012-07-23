#ifndef VALIDATOR_H
#define VALIDATOR_H

#include <QObject>
#include "common.h"
#include "logic/model.h"

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
    bool validateModel(logic::model* m);

signals:
    //! Вызывается из validateLink() в случае невозможности создать связь
    void wrongLink(ItemType fromType, ItemType toType);
    //! Вызывается из validateModel() в случае наличия логических ошибок в модели
    void modelError(logic::exceptions::LogicException& ex);
};

#endif // VALIDATOR_H
