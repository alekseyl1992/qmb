#include "validator.h"

Validator::Validator()
{
    //заполняем список неподдерживаемых связей
   /* invalidLinks.insert(ItemType::Generator, ItemType::Generator);
    invalidLinks.insert(ItemType::Terminator, ItemType::Terminator);
    invalidLinks.insert(ItemType::Terminator, ItemType::Generator);
    invalidLinks.insert(ItemType::Terminator, ItemType::Handler);
    invalidLinks.insert(ItemType::Terminator, ItemType::Queue);*/
}

Validator &Validator::inst()
{
    static Validator validator;
    return validator;
}

bool Validator::validateLink(ItemType fromType, ItemType toType)
{
    if(fromType == ItemType::Terminator || toType == ItemType::Generator)
    {
        emit wrongLink(fromType, toType);
        return false;
    }
    else
        return true;
}
