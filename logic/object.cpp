#include "object.h"
#include "model.h"

namespace logic
{
    object::object(ItemType type, std::string _name, int id) :
        item_type(type),
        name(_name),
        parent(nullptr),
        id(id),
        global_id(id),
        cur_req(nullptr),
        moveable_request_flag(false),
        freedom_flag(true)
    { }

    object::object(const object &obj):
        item_type(obj.item_type),
        name(obj.name),
        parent(obj.parent),
        id(obj.id),
        global_id(obj.global_id),
        inputs(obj.inputs),
        outputs(obj.outputs),
        cur_req(obj.cur_req),
        moveable_request_flag(obj.moveable_request_flag),
        freedom_flag(obj.freedom_flag)
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

    void object::set_parent(model* _parent)
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

    bool object::is_completed()
    {
        return is_free();
    }

    int object::get_event_time() const
    {
        return static_cast<int>(get_now_time() - parent->get_start_time());
    }

} //end namespace logic
