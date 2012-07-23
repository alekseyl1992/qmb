#include "collector.h"

#include <time.h>

namespace logic
{
    collector::collector(std::string name, int id) :
        object(ItemType::Collector, name, id)
    { }

    collector::collector(collector &col) :
        object(col)
    { }

    request* collector::get_request()
    {
        std::lock_guard<std::mutex> lk(item_mutex);

        moveable_request_flag = false;
        freedom_flag = true;
        return cur_req;
    }

    void collector::add(request* req)
    {
        std::lock_guard<std::mutex> lk(item_mutex);

        freedom_flag = false;
        cur_req = req;
        moveable_request_flag = true;

        qDebug() << cur_req->get_id().str_reqID().c_str() << "is in Collector " << get_id();
    }

    void collector::move_request()
    {
        srand(time(NULL));
        const int i = rand() % inputs_count();
        auto it = inputs.begin();
        advance(it, i);

        if ((*it)->is_moveable() && this->is_free())
        {
            this->add((*it)->get_request());
        }
    }

} //end namespace logic
