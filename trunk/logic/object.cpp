#include "object.h"
#include "model.h"

namespace logic
{
    object::object(ItemType type, ull_t id) :
		item_type(type),
		parent(nullptr), 
		id(id), 
        global_id(id),
		input(nullptr), 
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
		input(obj.input), 
		cur_req(obj.cur_req),
		moveable_request_flag(obj.moveable_request_flag),
		freedom_flag(obj.freedom_flag),
		name(obj.name)
    { }

    object::~object()
    {
	}

	void object::connect_with(object* _source)
	{
		this->input = _source;
	}

	bool object::has_connection() const
	{
		return input == nullptr ? false : true;
	}

	void object::set_parrent(model* _parent)
	{
		this->parent = _parent;
	}

    void object::move_request()
	{
		if (this->input->is_moveable() && this->is_free())
		{
			this->add(input->get_request());
		}
	}

} //end namespace logic
