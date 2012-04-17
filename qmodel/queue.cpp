#include "queue.h"

namespace qmodel
{
	/**********************Implementation**********************/

	int queue::cur_id = 0;

	//constructors
    queue::queue():
		qmbObject(++cur_id), having_request_flag(false) 
	{ }
	
    queue::queue(const queue& q) :
		requests_in_queue(q.requests_in_queue), having_request_flag(q.having_request_flag) 
	{ }

	//assignment
    queue& queue::operator=(const queue& q) {
		if (this == &q)
			return *this;
		requests_in_queue.clear();
		requests_in_queue.assign(q.requests_in_queue.begin(), q.requests_in_queue.end());

		having_request_flag = q.having_request_flag;
		return *this;
	}

	//adding request to the queue
	void queue::add(request req) {
		std::lock_guard<std::mutex> lock(queue_mutex);
        sLog << "-Request[" << req.get_id() << "] was put to the queue " << get_id() << endl;
		requests_in_queue.push_back(req);
		having_request_flag = true;
	}

	//getting next request
	request queue::get_first() {
		std::lock_guard<std::mutex> lock(queue_mutex);
		request res = *(requests_in_queue.begin());
		requests_in_queue.pop_front();
		if(requests_in_queue.size() == 0)
			having_request_flag = false;
		return res;
	}

	//getting last request
	request queue::get_last() {
		std::lock_guard<std::mutex> lock(queue_mutex);
		request res = *(requests_in_queue.end() - 1);
		requests_in_queue.pop_back();
		if(requests_in_queue.size() == 0)
			having_request_flag = false;
		return res;
	}

} //end namespace qmodel
