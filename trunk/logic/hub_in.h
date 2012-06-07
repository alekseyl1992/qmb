#ifndef HUB_IN_H
#define HUB_IN_H

#include <vector>

#include "object.h"

namespace logic
{
	class hub_in : public object
	{
	public:
		hub_in(ull_t id = 0);
		hub_in(const hub_in& ih);
		virtual ~hub_in();

		virtual void add(const request& req);
		
	private:
		std::vector<object*> inputs;
	};

}  

#endif // !HUB_IN_H