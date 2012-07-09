#ifndef COLLECTOR_H
#define COLLECTOR_H

#include <list>

#include "object.h"

namespace logic
{
    class collector : public object
    {
    public:
        collector(int id);
        collector(collector& col);

        virtual ~collector() { }

        int inputs_count() const;

        virtual request* get_request();
        virtual void add(request* req);
        virtual void move_request();
        virtual bool is_completed();
    };

} //end namespace logic

#endif // COLLECTOR_H
