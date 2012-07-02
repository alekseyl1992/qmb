#include "request.h"

namespace logic
{
    request::request(ull_t gen_id, ull_t _id) : id(gen_id, _id)
    { }

    request::request(const request& req) : id(req.id)
    { }

} //end namespace logic
