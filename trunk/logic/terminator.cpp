#include "terminator.h"
#include "object.h"
#include "model.h"

namespace logic
{

    terminator::terminator(model *parent, int id, int period) :
        object(parent, id), cur_req(), terminating_period(period), freedom_flag(true), count_of_terminated_requests(0)
    { }

    terminator::terminator(const terminator &t) :
        object(t.parent, t.id), cur_req(t.cur_req), terminating_period(t.terminating_period), freedom_flag(t.freedom_flag)//, count_of_terminated_requests(t.count_of_terminated_requests)
    { }

    terminator &terminator::operator =(const terminator &t)
    {
        if (this == &t)
            return *this;
        parent = t.parent;
        id = t.id;
        cur_req = t.cur_req;
        terminating_period = t.terminating_period;
        freedom_flag = t.freedom_flag;
        count_of_terminated_requests = t.count_of_terminated_requests;
        return *this;
    }

    void terminator::terminate(const request &req)
    {
        std::lock_guard<std::mutex> lk(terminator_mutex);

        freedom_flag = false;
        cur_req = req;

        std::this_thread::sleep_for(std::chrono::milliseconds(terminating_period));
        ++count_of_terminated_requests;

        freedom_flag = true;

        emit parent->reqTerminated(id, cur_req.get_id(), static_cast<int>(get_now_time() - parent->start_time));
        qDebug() << cur_req.get_id().__req_gen_id << "-" << cur_req.get_id().__req_id << " terminated in Terminator " << get_id();


    }
}
