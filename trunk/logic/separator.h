#ifndef SEPARATOR_H
#define COLLECTOR_H

#include <list>

#include "object.h"

namespace logic
{
    //данный класс работает по иному принципу, по принцпу "запрос на выход",
    //поэтому объекты модели, которые являются выходами разделителя (separator)
    //не должны крутиться в потоках.
    class separator : public object
    {
    public:
        separator(int id);
        separator(separator& sep);

        virtual ~separator() { }

        virtual request* get_request();
        virtual void add(request* req);
        virtual void move_request();    //работает по принципу "на выход"
        virtual bool is_completed();
    };

} //end namespace logic

#endif // COLLECTOR_H
