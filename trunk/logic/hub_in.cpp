#include "hub_in.h"
#include "request.h"

namespace logic
{

	hub_in::hub_in(ull_t id) :
		object(ItemType::HubIn, id)
	{ }

	hub_in::hub_in(const hub_in& ih):
		object(ih)
	{ }

	hub_in::~hub_in()
	{ }


	void hub_in::add(const request& req)
	{

	}

}