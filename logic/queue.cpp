﻿#include "queue.h"
#include "model.h"

namespace logic
{
    queue::queue(std::string name, int id, bool from_top):
        object(ItemType::Queue, name, id),
        to_get_from_top(from_top)
    { }

    queue::queue(const queue &q) :
        object(q),
        requests(q.requests),
        to_get_from_top(q.to_get_from_top)
    { }

    queue::~queue() { }

    void queue::make_cur_request()
    {
        if (to_get_from_top)
            cur_req = (requests.size() != 0) ? *requests.begin() : nullptr;
        else
            cur_req = (requests.size() != 0) ? *(requests.end() - 1) : nullptr;
    }

    void queue::add(request* req)
    {
        std::lock_guard<std::mutex> lk(item_mutex);

        requests.push_back(req);
        make_cur_request();

        moveable_request_flag = (requests.size() == 0) ? false : true;

        if (parent->is_simulating())
        {
            emit parent->reqQueued(id, requests.back()->get_id(), get_event_time());
            qDebug() << requests.back()->get_id().str_reqID().c_str() << " was put to the queue " << get_id();
        }
    }

    request* queue::get_first()
    {
        requests.pop_front();
        return cur_req;
    }

    request* queue::get_last()
    {
        requests.pop_back();
        return cur_req;
    }

    request* queue::get_request()
    {
        std::lock_guard<std::mutex> lk(item_mutex);

        request* res = to_get_from_top ? get_first() : get_last();
        make_cur_request();
        moveable_request_flag = (requests.size() == 0) ? false : true;
        return res;
    }

} //end namespace logic
