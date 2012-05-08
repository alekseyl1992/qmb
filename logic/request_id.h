#ifndef H_REQUEST_ID
#define H_REQUEST_ID

#include <iostream>
#include <sstream>

#include "../utility/simulationlog.h"
#include "../utility/common.h"

namespace logic
{
	struct request_id
	{
		request_id(): __req_gen_id(0), __req_id(0) { }
        request_id(ull_t req_gen_id, ull_t req_id): __req_gen_id(req_gen_id), __req_id(req_id) { }
        request_id(const request_id& req_id): __req_gen_id(req_id.__req_gen_id), __req_id(req_id.__req_id) { }

        request_id& operator= (const request_id& req_id) {
            if (this == &req_id)
                return *this;
            __req_gen_id = req_id.__req_gen_id;
            __req_id = req_id.__req_id;
			return *this;
		}

        ull_t __req_gen_id;
        ull_t __req_id;
	};

} //end namespace logic

std::basic_ostream<char>& operator<< (std::basic_ostream<char>& os, const logic::request_id &id);

#endif // !H_REQUEST_ID
