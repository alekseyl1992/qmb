#ifndef COLLECTOR_H
#define COLLECTOR_H

#include <list>

#include "object.h"

namespace logic
{
    class collector : public object
    {
    public:
        collector(std::string name, int id);
        collector(collector& col);

        virtual ~collector() { }

        virtual request* get_request();
        virtual void add(request* req);
        virtual void move_request();
    };

} //end namespace logic

#endif // COLLECTOR_H
