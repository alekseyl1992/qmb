﻿#include "terminator.h"
#include "model.h"

namespace logic
{
    terminator::terminator(std::string name, int id, int period) :
        object(ItemType::Terminator, name, id),
        terminating_period(period),
        count_of_terminated_requests(0)
    { }

    terminator::terminator(const terminator &t) :
        object(t),
        terminating_period(t.terminating_period),
        count_of_terminated_requests(t.count_of_terminated_requests)
    { }

    terminator::~terminator() { }

    void terminator::terminate(request* req)
    {
        cur_req = req;

        std::this_thread::sleep_for(std::chrono::milliseconds(terminating_period));

        if (parent->is_simulating())
        {
            emit parent->reqTerminated(id, cur_req->get_id(), get_event_time());
            qDebug() << cur_req->get_id().str_reqID().c_str() << " terminated in Terminator " << get_id();
        }

        delete cur_req;
        cur_req = nullptr;

        moveable_request_flag = true;
        freedom_flag = true;
        ++count_of_terminated_requests;
    }

    void terminator::add(request* req)
    {
        std::lock_guard<std::mutex> lk(item_mutex);

        moveable_request_flag = false;
        freedom_flag = false;
        this->terminate(req);
    }

    request* terminator::get_request()
    {
        std::lock_guard<std::mutex> lk(item_mutex);
        return new request();
    }

} //end namespace logic
