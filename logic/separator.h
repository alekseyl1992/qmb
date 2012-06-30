#ifndef SEPARATOR_H
#define COLLECTOR_H

#include <list>

#include "object.h"

namespace logic
{
    //������ ����� �������� �� ����� ��������, �� ������� "������ �� �����",
    //������� ������� ������, ������� �������� �������� ����������� (separator)
    //�� ������ ��������� � �������.
    class separator : public object
    {
    public:
        separator(ull_t id);
        separator(separator& sep);

        virtual ~separator() { }

        //virtual void set_input(object* _source);  //inherited
        virtual void set_output(object* _dest);

        //virtual bool has_input() const;  //inherited
        virtual bool has_output() const;

        int outputs_count() const;

        virtual request* get_request();
        virtual void add(request* req);
        virtual void move_request();    //�������� �� �������� "�� �����"
        virtual bool is_completed();

    private:
        //object* input; //inherited
        std::list<object*> outputs;
    };

} //end namespace logic

#endif // COLLECTOR_H
