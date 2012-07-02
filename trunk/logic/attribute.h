#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

#include <deque>
#include <vector>
#include <string>
#include <QObject>

namespace logic
{
    class value
    {
    public:
        value(std::string name) :
            val_name(name),
            selection_flag(false)
        { }

        value(const value& val) :
            val_name(val.val_name),
            selection_flag(val.selection_flag)
        { }

        ~value() { }

        void select()
        { selection_flag = true; }

    private:
        std::string val_name;
        bool selection_flag; //флаг, показывающий, что значение атрибута выбрано
    };

    class attribute
    {
    public:
        attribute(int id, std::string name, std::deque<value> values);
        attribute(const attribute& attr);
        ~attribute() { }

        int get_id() const
        { return _id; }

        std::string get_name() const
        { return _name; }

        std::deque<value> get_values() const
        { return _values; }

        void select(int val_id);

    private:
        int _id;
        std::string _name;
        std::deque<value> _values;
    };

    class attributes_list : public QObject
    {
        Q_OBJECT

    public:
        void add(attribute& a);
        void erase(int attr_id);
        attribute& get(int attr_id);

        int size() const;

        void select(int attr_id, int val_id);
    private:
        std::vector<attribute> _attrs;
    };
}


#endif // ATTRIBUTE_H
