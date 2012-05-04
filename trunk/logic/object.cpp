#include "object.h"

namespace logic
{
    object::object(model* p) : parent(p) { }
    object::object(model* p, int cur_id) : parent(p), id(cur_id) { }

    object::object(const object &obj): parent(obj.parent), id(obj.id)
    { }

    object::~object()
    { }
}
