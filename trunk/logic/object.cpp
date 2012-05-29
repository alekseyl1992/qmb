#include "object.h"

namespace logic
{
    object::object(model* p, int id) : parent(p), id(id) { }

    object::object(const object &obj): parent(obj.parent), id(obj.id)
    { }

    object::~object()
    { }
}
