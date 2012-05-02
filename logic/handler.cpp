#include "handler.h"

namespace logic
{
	int handler::cur_id = 0;

	//Constructor
	handler::handler(int _handlePeriod): 
        object(++cur_id), cur_req(nullptr), handling_period(_handlePeriod), freedom_flag(true)
	{ }

	handler::handler(const handler& h) : 
		cur_req(h.cur_req), handling_period(h.handling_period), freedom_flag(h.freedom_flag) 
	{ }

	//assignment
    handler& handler::operator=(const handler& h) {
		if (this == &h)
			return *this;
		cur_req = h.cur_req;
		handling_period = h.handling_period;
		freedom_flag = h.freedom_flag;
		return *this;
	}

	//Send request to handler
    void handler::handle(request req) {
		std::lock_guard<std::mutex> lk(handler_mutex);
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
        std::stringstream ss;
        ss << "----Request[" << cur_req->get_id() << "] finished handling in handler " << get_id();
        sLog.writeLine(ss.str());
		freedom_flag = true;
		delete cur_req;
	}

} //end namespace logic
