﻿#ifndef SEPARATOR_H
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

        //virtual void set_input(object* _source);  //inherited
        virtual void set_output(object* _dest);

        //virtual bool has_input() const;  //inherited
        virtual bool has_output() const;

        //virtual std::list<object*> input_connection() const;    //inherited
        virtual std::list<object*> output_connection() const;

        int outputs_count() const;

        virtual request* get_request();
        virtual void add(request* req);
        virtual void move_request();    //работает по принципу "на выход"
        virtual bool is_completed();

    private:
        //object* input; //inherited
        std::list<object*> outputs;
    };

} //end namespace logic

#endif // COLLECTOR_H
