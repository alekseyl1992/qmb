#include "attribute.h"

namespace logic
{
    attribute::attribute(int id, std::string name, std::deque<logic::attribute::value> values) :
        _id(id),
        _name(name),
        _values(values)
    { }

    attribute::attribute(attribute& attr) :
        _id(attr._id),
        _name(attr._name),
        _values(attr._values)
    { }

} //end namespace logic
