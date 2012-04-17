#include "request.h"

namespace qmodel
{
	/**********************Implementation**********************/
    request::request(int gen_id, int _id) : id(gen_id, _id)
	{ }

    request::request(const request& req) : id(req.id)
	{ }

    request& request::operator=(const request& r) {
		if (&r == this)
			return *this;
		id = r.id;
		return *this;
	}

} //end namespace qmodel
