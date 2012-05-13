#include "request_id.h"
#include <sstream>

namespace logic
{
    request_id::request_id(): __req_gen_id(0), __req_id(0) { }
    request_id::request_id(ull_t req_gen_id, ull_t req_id): __req_gen_id(req_gen_id), __req_id(req_id) { }
    request_id::request_id(const request_id& req_id): __req_gen_id(req_id.__req_gen_id), __req_id(req_id.__req_id) { }

    request_id& request_id::operator= (const request_id& req_id) {
        if (this == &req_id)
            return *this;
        __req_gen_id = req_id.__req_gen_id;
        __req_id = req_id.__req_id;
        return *this;
    }

    const char *request_id::txt_reqID() const
    {
        std::stringstream ss;
        ss << __req_gen_id << ":" << __req_id;
        return ss.str().c_str();
    }
} //end namespace logic

std::basic_ostream<char>& operator<< (std::basic_ostream<char>& os, const logic::request_id &id)
{
   //output of the id
   os << id.__req_gen_id << "-" << id.__req_id;
   return os;
}
