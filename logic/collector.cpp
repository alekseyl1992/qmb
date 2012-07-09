#include "collector.h"

#include <time.h>

namespace logic
{
    collector::collector(int id) :
        object(ItemType::Collector, id)
    { }

    collector::collector(collector &col) :
        object(col)
    { }

    void collector::set_input(object* _source)
    {
        inputs.push_back(_source);
    }

    bool collector::has_input() const
    {
        return inputs.size() == 0 ? false : true;
    }

    std::list<object *> collector::input_connection() const
    {
        return inputs;
    }

    int collector::inputs_count() const
    {
        return static_cast<int>(inputs.size());
    }


    //actual logic

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

    bool collector::is_completed()
    {
        return is_free();
    }



} //end namespace logic
