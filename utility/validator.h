#ifndef VALIDATOR_H
#define VALIDATOR_H

//#include <QObject>
#include "common.h"

/*! Класс предназначен для проверки модели на валидность.
  * Реализует паттерн singleton
  */
class Validator : public QObject
{
    Q_OBJECT

private:
    Validator();

    //QMultiMap<ItemType, ItemType> invalidLinks;

public:
    static Validator& inst();
    bool validateLink(ItemType fromType, ItemType toType); //!< Формирует список неподдерживаемых моделью связей

signals:
    void wrongLink(ItemType fromType, ItemType toType);
};

#endif // VALIDATOR_H
