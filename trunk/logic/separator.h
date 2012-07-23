#ifndef SEPARATOR_H
#define COLLECTOR_H

#include <list>
#include <QtScript>

#include "object.h"

namespace logic
{
    //данный класс работает по иному принципу, по принцпу "запрос на выход",
    //поэтому объекты модели, которые являются выходами разделителя (separator)
    //не должны крутиться в потоках.
    class separator : public object
    {
    public:
        separator(std::string name, int id, QString script);
        separator(separator& sep);

        virtual ~separator();

        virtual request* get_request();
        virtual void add(request* req);
        virtual void move_request();    //работает по принципу "на выход"

    private:
        QScriptEngine* engine;
        QString script;
    };

} //end namespace logic

#endif // COLLECTOR_H
