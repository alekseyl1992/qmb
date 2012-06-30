#ifndef COLLECTOR_H
#define COLLECTOR_H

#include <list>

#include "object.h"

namespace logic
{
    class collector : public object
    {
    public:
        collector(ull_t id);
        collector(collector& col);

        virtual ~collector() { }

        virtual void set_input(object* _source);		//!< Служит для соединения объекта с входом
        //virtual void set_output(object* _dest);  //inherited

        virtual bool has_input() const;
        //virtual bool has_output() const;  //inherited

        int inputs_count() const;

        virtual request* get_request();
        virtual void add(request* req);
        virtual void move_request();
        virtual bool is_completed();

    private:
        std::list<object*> inputs;
        //object* output; //inherited
    };

} //end namespace logic

#endif // COLLECTOR_H
