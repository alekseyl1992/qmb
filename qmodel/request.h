#ifndef H_REQUEST
#define H_REQUEST

#include <ostream>
#include "../simulationlog.h"

namespace qmodel
{
	template<typename IntType = long>
	struct request_id
	{
		request_id(): __req_gen_id(0), __req_id(0) { }
		request_id(IntType req_gen_id, IntType req_id): __req_gen_id(req_gen_id), __req_id(req_id) { }
		request_id(const request_id& req): __req_gen_id(req.__req_gen_id), __req_id(req.__req_id) { }

		request_id& operator= (const request_id& req)
		{
            if (this = &req)
                return *this;
			__req_gen_id = req.__req_gen_id; 
			__req_id = req.__req_id;
			return *this;
		}

		IntType __req_gen_id;
		IntType __req_id;
	};


	/*************************************************************************************/
	template<typename Type = int>
	class request
	{
	public:
		request(int gen_id, int _id, Type _content = NULL) : content(_content), id(gen_id, _id) { }
        request(const request& req) : content(req.content), id(req.id) { }
		request& operator=(const request& r) { content = r.content; id = r.id; return *this; }
		~request() { }

		Type get_content() const { return content; }
		request_id<> get_id() const { return id; }

	private:
		Type content;
		request_id<> id;
	};

} //end namespace qmodel

//output of the id
template<typename IntType>
SimulationLog& operator<< (SimulationLog& os, qmodel::request_id<IntType> id)
{
	os << id.__req_gen_id << "-" << id.__req_id;
	return os;
}

#endif // !H_REQUEST
