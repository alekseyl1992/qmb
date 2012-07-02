#include "attribute.h"

#include <algorithm>

namespace logic
{
    attribute::attribute(int id, std::string name, std::deque<value> values) :
        _id(id),
        _name(name),
        _values(values)
    { }

    attribute::attribute(const attribute &attr) :
        _id(attr._id),
        _name(attr._name),
        _values(attr._values)
    { }

    void attribute::select(int val_id)
    {
        auto it = _values.begin() + (val_id - 1);
        it->select();
    }


    /******************\attributes_list\******************/

    void attributes_list::add(attribute &a)
    {
        _attrs.push_back(a);
    }

    void attributes_list::erase(int attr_id)
    {
        _attrs.erase(_attrs.begin() + (attr_id - 1));
    }

    attribute& attributes_list::get(int attr_id)
    {
        auto it = std::find_if(_attrs.begin(), _attrs.end(), [&attr_id](attribute& attr)
        {
            return attr.get_id() == attr_id;
        });
        return *it;
    }

    int attributes_list::size() const
    {
        return static_cast<int>(_attrs.size());
    }

    void attributes_list::select(int attr_id, int val_id)
    {
        auto it = std::find_if(_attrs.begin(), _attrs.end(), [&attr_id](attribute& attr)
        {
            return attr.get_id() == attr_id;
        });

        it->select(val_id);
    }

} //end namespace logic
