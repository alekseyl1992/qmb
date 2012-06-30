#include "separator.h"

#include <time.h>

namespace logic
{
    separator::separator(ull_t id) :
        object(ItemType::Separator, id)
    { }

    separator::separator(separator &sep) :
        object(sep)
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
        std::lock_guard<std::mutex> lk(item_mutex);

        moveable_request_flag = false;
        freedom_flag = true;
        return cur_req;
    }

    void separator::add(request* req)
    {
        std::lock_guard<std::mutex> lk(item_mutex);

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

        if (this->has_request())
        {
            srand(time(NULL));
            const int i = rand() % outputs_count();
            auto it = outputs.begin();
            advance(it, i);

            //помещаем запрос в один из выходов в случае если на выходе НЕ коллектор
            if (this->is_moveable() && (*it)->is_free() && (*it)->get_type() != ItemType::Collector)
            {
                (*it)->add(this->get_request());
            }
        }
    }

    bool separator::is_completed()
    {
        return is_free();
    }



} //end namespace logic
