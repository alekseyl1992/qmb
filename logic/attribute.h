#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

#include <deque>
#include <string>
#include <QObject>

namespace logic
{
    class value : public QObject
    {
        Q_OBJECT

    public:
        value(int id, std::string name) :
            val_name(name)
        { }

        value(value& val) :
            val_name(val.val_name)
        { }

        ~value() { }

    private:
        std::string val_name;
    };

    class attribute : public QObject
    {
        Q_OBJECT

    public:
        attribute(int id, std::string name, std::deque<value> values);
        attribute(attribute& attr);
        ~attribute() { }

    private:
        int _id;
        std::string _name;
        std::deque<value> _values;
    };
}


#endif // ATTRIBUTE_H
