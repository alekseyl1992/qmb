#include "handler.h"
#include "model.h"

namespace logic
{
	//Constructor
    handler::handler(model* parent, int id, int _handlePeriod):
        object(parent, id), cur_req(nullptr), handling_period(_handlePeriod), freedom_flag(true)
	{ }

	handler::handler(const handler& h) : 
        object(h), cur_req(h.cur_req), handling_period(h.handling_period), freedom_flag(h.freedom_flag)
	{ }

	//assignment
    handler& handler::operator=(const handler& h) {
		if (this == &h)
			return *this;
        parent = h.parent;
		cur_req = h.cur_req;
		handling_period = h.handling_period;
		freedom_flag = h.freedom_flag;
		return *this;
	}

	//Send request to handler
    void handler::handle(request req) {
		std::lock_guard<std::mutex> lk(handler_mutex);
        emit parent->reqBeganHandling(id, req.get_id());

		freedom_flag = false;
		cur_req = new request(req);

        std::stringstream ss;
        ss << "---Request[" << req.get_id() << "] was put to the handler " << get_id();
        sLog.writeLine(ss.str());

		std::this_thread::sleep_for(std::chrono::milliseconds(handling_period));
        finish_handling();
	}

	//finish the request
    void handler::finish_handling() {
        freedom_flag = true;

        emit parent->reqFinishedHandling(id, cur_req->get_id());

        std::stringstream ss;
        ss << "----Request[" << cur_req->get_id() << "] finished handling in handler " << get_id();
        sLog.writeLine(ss.str());

        delete cur_req;
	}

} //end namespace logic
