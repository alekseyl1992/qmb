#include "queue.h"
#include "model.h"

namespace logic
{
    int queue::cur_id = 0;

	//constructors
    queue::queue(model* parent):
        object(parent, ++cur_id), having_request_flag(false)
	{ }

    queue::queue(const queue &q) :
        object(q), requests_in_queue(q.requests_in_queue), having_request_flag(q.having_request_flag)
	{ }

	queue::~queue() {
        //queue_mutex.native_handle();
	}

	//assignment
	queue& queue::operator=(const queue& q) {
		if (this == &q)
			return *this;
        parent = q.parent;
		requests_in_queue.clear();
		requests_in_queue.assign(q.requests_in_queue.begin(), q.requests_in_queue.end());

		having_request_flag = q.having_request_flag;
		return *this;
	}

	//adding request to the queue
	void queue::add(request req) {
        std::lock_guard<std::mutex> lk(queue_mutex);

        requests_in_queue.push_back(req);
        if(requests_in_queue.size() == 0)
            having_request_flag = false;
        else
            having_request_flag = true;

        emit parent->reqQueued(id, req.get_id());

        std::stringstream ss;
        ss << "-Request[" << req.get_id() << "] was put to the queue " << get_id();
        sLog.writeLine(ss.str());
	}

	//getting next request
	request queue::get_first() {
        std::lock_guard<std::mutex> lk(queue_mutex);
		request res = *(requests_in_queue.begin());
		requests_in_queue.pop_front();
		if(requests_in_queue.size() == 0)
			having_request_flag = false;
		else
			having_request_flag = true;
		return res;
	}

	//getting last request
	request queue::get_last() {
        std::lock_guard<std::mutex> lk(queue_mutex);
		request res = *(requests_in_queue.end() - 1);
		requests_in_queue.pop_back();
		if(requests_in_queue.size() == 0)
			having_request_flag = false;
		else
			having_request_flag = true;
		return res;
	}

} //end namespace logic
