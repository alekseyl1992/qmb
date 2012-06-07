#ifndef H_REQUEST
#define H_REQUEST

#include "request_id.h"

namespace logic
{
	class request
	{
	public:
		request(ull_t gen_id = 0, ull_t _id = 0);
		request(const request& req);
		~request() { }

        request_id get_id() const { return id; }

	private:
        request_id id;
	};

} //end namespace logic

#endif // !H_REQUEST
