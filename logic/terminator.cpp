#include "terminator.h"
#include "model.h"

namespace logic
{
    terminator::terminator(ull_t id, int period) :
		object(ItemType::Terminator, id), 
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
            emit parent->reqTerminated(id, cur_req->get_id(), static_cast<int>(get_now_time() - parent->start_time));
            //std::cout << cur_req->get_id().str_reqID() << " terminated in Terminator " << get_id() << endl;
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
