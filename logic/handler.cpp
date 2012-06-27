#include "handler.h"
#include "model.h"

namespace logic
{
    handler::handler(ull_t id, int _handlePeriod):
		object(ItemType::Handler, id), 
		handling_period(_handlePeriod), 
		count_of_handled_requests(0)
	{ }

	handler::handler(const handler& h) : 
        object(h), 
		handling_period(h.handling_period), 
		count_of_handled_requests(h.count_of_handled_requests)
	{ }

	handler::~handler() { }

    void handler::handle(request* req)
	{
        cur_req = req;

		if (parent->is_simulating())
		{
            emit parent->reqBeganHandling(id, cur_req->get_id(), get_event_time());
            qDebug() << cur_req->get_id().str_reqID().c_str() << " was put to the handler " << get_id();
		}

        std::this_thread::sleep_for(std::chrono::milliseconds(handling_period));

		if (parent->is_simulating())
		{
            emit parent->reqFinishedHandling(id, cur_req->get_id(), get_event_time());
            qDebug() << cur_req->get_id().str_reqID().c_str() << " finished handling in handler " << get_id();
		}

		moveable_request_flag = true;
		++count_of_handled_requests; //handled
    }

    request* handler::get_request()
    {
        std::lock_guard<std::mutex> lk(item_mutex);

		freedom_flag = true;
		moveable_request_flag = false;
        return cur_req;
    }

	void handler::add(request* req)
	{
		std::lock_guard<std::mutex> lk(item_mutex);

		freedom_flag = false;
		moveable_request_flag = false;
		this->handle(req);
	}

} //end namespace logic
