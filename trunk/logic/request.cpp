#include "request.h"

namespace logic
{
    ull_t request::num_generated = 0;
    ull_t request::num_deleted = 0;

    request::request(int gen_id, ull_t _id)
        : id(gen_id, _id)
    {
        ++num_generated;
    }

    request::request(const request& req)
        : id(req.id)
    { }

    request::~request()
    {
        ++num_deleted;
    }


} //end namespace logic
