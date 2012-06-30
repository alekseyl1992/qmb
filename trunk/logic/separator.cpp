#include "separator.h"

#include <time.h>

namespace logic
{
    separator::separator(ull_t id) :
        object(ItemType::Collector, id)
    { }

    separator::separator(separator &col) :
        object(col)
    { }

    void separator::set_output(object* _dest)
    {
        outputs.push_back(_dest);
    }

    bool separator::has_output() const
    {
        return outputs.size() == 0 ? false : true;
    }

    int separator::outputs_count() const
    {
        return static_cast<int>(outputs.size());
    }


    //actual logic

    request* separator::get_request()
    {
        moveable_request_flag = false;
        freedom_flag = true;
        return cur_req;
    }

    void separator::add(request* req)
    {
        freedom_flag = false;
        cur_req = req;
        moveable_request_flag = true;
    }

    void separator::move_request()
    {
        //вытаскиваем запрос из input
        if (input->is_moveable() && this->is_free())
        {
            this->add(input->get_request());
        }

        srand(time(NULL));
        const int i = rand() % outputs_count();
        auto it = outputs.begin();
        advance(it, i);

        //помещаем запрос в один из выходов
        if (this->is_moveable() && (*it)->is_free())
        {
            (*it)->add(this->get_request());
        }
    }

    bool separator::is_completed()
    {
        return is_free();
    }



} //end namespace logic
