#include "object.h"
#include "model.h"

namespace logic
{
    object::object(ItemType type, int id) :
        item_type(type),
        parent(nullptr),
        id(id),
        global_id(id),
        //input(nullptr),
        //output(nullptr),
        cur_req(nullptr),
        moveable_request_flag(false),
        freedom_flag(true),
        name("")
    { }

    object::object(const object &obj):
        item_type(obj.item_type),
        parent(obj.parent),
        id(obj.id),
        global_id(obj.global_id),
        //input(obj.input),
        //output(obj.output),
        cur_req(obj.cur_req),
        moveable_request_flag(obj.moveable_request_flag),
        freedom_flag(obj.freedom_flag),
        name(obj.name)
    { }

    object::~object()
    {
    }

    bool object::has_request() const
    {
        return cur_req == nullptr ? false : true;
    }

    void object::set_input(object* _source)
    {
        this->inputs.push_back(_source);
    }

    void object::set_output(object* _dest)
    {
        this->outputs.push_back(_dest);
    }

    bool object::has_input() const
    {
        return inputs.size() == 0 ? false : true;
    }

    bool object::has_output() const
    {
        return outputs.size() == 0 ? false : true;
    }

    std::list<object *> object::input_connection() const
    {
        return inputs;
    }

    std::list<object *> object::output_connection() const
    {
        return outputs;
    }

    int object::inputs_count() const
    {
        return static_cast<int>(inputs.size());
    }

    int object::outputs_count() const
    {
        return static_cast<int>(outputs.size());
    }

    object *object::input() const
    {
        return *(inputs.begin());
    }

    object *object::output() const
    {
        return *(outputs.begin());
    }

    void object::set_parrent(model* _parent)
    {
        this->parent = _parent;
    }

    void object::move_request()
    {
        if (this->input()->is_moveable() && this->is_free())
        {
            this->add(input()->get_request());
        }
    }

    int object::get_event_time() const
    {
        return static_cast<int>(get_now_time() - parent->get_start_time());
    }



} //end namespace logic
