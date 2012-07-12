#include <sstream>
#include "request_id.h"

namespace logic
{
    request_id::request_id(int req_gen_id, ull_t req_id):
        __req_gen_id(req_gen_id),
        __req_id(req_id)
    { }
    request_id::request_id(const request_id& req_id):
        __req_gen_id(req_id.__req_gen_id),
        __req_id(req_id.__req_id)
    { }

    std::string request_id::str_reqID() const
    {
        std::stringstream ss;
        ss << __req_gen_id << ":" << __req_id;
        return ss.str();
    }

} //end namespace logic
