#include "handler.h"
#include "model.h"
#include "terminator.h"

namespace logic
{
	//Constructor
    handler::handler(model* parent, int id, int _handlePeriod):
        object(parent, id), cur_req(nullptr), handling_period(_handlePeriod), freedom_flag(true), handled_flag(false), counter_of_handled_requests(0)
	{ }

	handler::handler(const handler& h) : 
        object(h), cur_req(h.cur_req), handling_period(h.handling_period), freedom_flag(h.freedom_flag), handled_flag(h.handled_flag), counter_of_handled_requests(h.counter_of_handled_requests)
	{ }

	//assignment
    handler& handler::operator=(const handler& h) {
		if (this == &h)
			return *this;
        parent = h.parent;
		cur_req = h.cur_req;
		handling_period = h.handling_period;
		freedom_flag = h.freedom_flag;
        handled_flag = h.handled_flag;
        counter_of_handled_requests = h.counter_of_handled_requests;
		return *this;
	}

	//Send request to handler
    void handler::handle(const request& req) {
        freedom_flag = false;
		std::lock_guard<std::mutex> lk(handler_mutex);



		cur_req = new request(req);

        emit parent->reqBeganHandling(id, req.get_id());
        /*std::stringstream ss;
        ss << "---Request[" << req.get_id() << "] was put to the handler " << get_id();
        sLog.writeLine(ss.str());
*/
        qDebug() << cur_req->get_id().__req_gen_id << "-" << cur_req->get_id().__req_id << " was put to the handler " << get_id();
		std::this_thread::sleep_for(std::chrono::milliseconds(handling_period));

        ++counter_of_handled_requests; //handled
        handled_flag = true;

        emit parent->reqFinishedHandling(id, cur_req->get_id());

       /* std::stringstream ss;
        ss << "----Request[" << cur_req->get_id() << "] finished handling in handler " << get_id();
        sLog.writeLine(ss.str());*/
        qDebug() << cur_req->get_id().__req_gen_id << "-" << cur_req->get_id().__req_id << " finished handling in handler " << get_id();
    }

    request handler::get_current_request()
    {
        std::lock_guard<std::mutex> lk(handler_mutex);

        request res(*cur_req);
        //delete cur_req;
        //cur_req = nullptr;
        freedom_flag = true;
        handled_flag = false;
        return res;
    }

} //end namespace logic
