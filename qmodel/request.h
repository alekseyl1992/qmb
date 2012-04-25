#ifndef H_REQUEST
#define H_REQUEST

#include <ostream>
#include <numeric>

#include "exceptions.h"
#include "request_id.h"


namespace qmodel
{
	class request
	{
	public:
		request(int gen_id, int _id);
		request(const request& req);
		request& operator=(const request& r);
		~request() { }

		//gets the request id
		request_id<> get_id() const { return id; }

	private:
		request_id<> id;
	};

} //end namespace qmodel

#endif // !H_REQUEST
