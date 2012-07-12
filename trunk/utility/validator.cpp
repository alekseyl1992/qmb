#include "validator.h"

Validator::Validator()
{

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

bool Validator::validateModel(logic::model* m)
{
    try
    {
        return m->is_valid();
    }
    catch (logic::exceptions::LogicException& ex)
    {
        emit modelError(ex);
    }
    return false;
}
