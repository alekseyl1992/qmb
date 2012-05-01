#ifndef H_REQUEST_ID
#define H_REQUEST_ID

#include <iostream>
#include <sstream>
#include "../simulationlog.h"
typedef unsigned long long IntType;

namespace qmodel
{
	struct request_id
	{
		request_id(): __req_gen_id(0), __req_id(0) { }
		request_id(IntType req_gen_id, IntType req_id): __req_gen_id(req_gen_id), __req_id(req_id) { }
		request_id(const request_id& req): __req_gen_id(req.__req_gen_id), __req_id(req.__req_id) { }

		request_id& operator= (const request_id& req) {
			__req_gen_id = req.__req_gen_id; 
			__req_id = req.__req_id;
			return *this;
		}

		IntType __req_gen_id;
		IntType __req_id;
	};

} //end namespace qmodel

std::basic_ostream<char>& operator<< (std::basic_ostream<char>& os, const qmodel::request_id &id);

#endif // !H_REQUEST_ID
