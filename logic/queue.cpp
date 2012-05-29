#include "queue.h"
#include "model.h"

namespace logic
{
	//constructors
    queue::queue(model* parent, int id):
        object(parent, id), having_request_flag(false)
	{ }

    queue::queue(const queue &q) :
        object(q), requests_in_queue(q.requests_in_queue), having_request_flag(q.having_request_flag)
	{ }

    queue::~queue() { }

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
    void queue::add(const request& req) {
        std::lock_guard<std::mutex> lk(queue_mutex);

        requests_in_queue.push_back(req);

        having_request_flag = (requests_in_queue.size() == 0) ? false : true;

        emit parent->reqQueued(id, req.get_id(), static_cast<int>(get_now_time() - parent->start_time));
        qDebug() << req.get_id().__req_gen_id << "-" << req.get_id().__req_id << " was put to the queue " << get_id();
	}

	//getting next request
	request queue::get_first() {
        std::lock_guard<std::mutex> lk(queue_mutex);
		request res = *(requests_in_queue.begin());
		requests_in_queue.pop_front();

        having_request_flag = (requests_in_queue.size() == 0) ? false : true;

		return res;
	}

	//getting last request
	request queue::get_last() {
        std::lock_guard<std::mutex> lk(queue_mutex);
		request res = *(requests_in_queue.end() - 1);
		requests_in_queue.pop_back();

        having_request_flag = (requests_in_queue.size() == 0) ? false : true;

		return res;
	}

} //end namespace logic
