#ifndef VALIDATOR_H
#define VALIDATOR_H

//#include <QObject>
#include "common.h"

/*! ����� ������������ ��� �������� ������ �� ����������.
  * ��������� ������� singleton
  */
class Validator : public QObject
{
    Q_OBJECT

private:
    Validator();

    //QMultiMap<ItemType, ItemType> invalidLinks;

public:
    static Validator& inst();
    bool validateLink(ItemType fromType, ItemType toType); //!< ��������� ������ ���������������� ������� ������

signals:
    void wrongLink(ItemType fromType, ItemType toType);
};

#endif // VALIDATOR_H
