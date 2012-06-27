﻿#include "object.h"
#include "model.h"

namespace logic
{
    object::object(ItemType type, ull_t id) :
		item_type(type),
		parent(nullptr), 
		id(id), 
        global_id(id),
        //input(nullptr),
        output(nullptr),
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
        output(obj.output),
		cur_req(obj.cur_req),
		moveable_request_flag(obj.moveable_request_flag),
		freedom_flag(obj.freedom_flag),
		name(obj.name)
    { }

    object::~object()
    {
	}

    void object::set_input(object* _source)
	{
        this->inputs.push_back(_source);
	}

    void object::set_output(object* _dest)
    {
        this->output = _dest;
    }

    bool object::has_input() const
	{
        return inputs.size() == 0 ? false : true;
	}

    bool object::has_output() const
    {
        return output == nullptr ? false : true;
    }

	void object::set_parrent(model* _parent)
	{
		this->parent = _parent;
	}

    void object::move_request()
	{
        srand(time(NULL));
        int i = rand() % inputs_count();

        if (this->inputs[i]->is_moveable() && this->is_free())
		{
            this->add(inputs[i]->get_request());
        }
    }

    int object::get_event_time() const
    {
        return static_cast<int>(get_now_time() - parent->get_start_time());
    }



} //end namespace logic
