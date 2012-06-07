#ifndef TERMINATOR_H
#define TERMINATOR_H

#include "object.h"
#include "request.h"

namespace logic
{
    class model;

	//! ����� �����������. �������� ��������� logic::model.
    /*!
     * ������������ ����� ������, ����������� ��� �������� ��������� (��������) ��
     * ������ logic::model. ��������� "�����" ��������� � ������.
     */

    class terminator : public object
    {
    public:
        terminator(ull_t id = 0, int period = 0);
        terminator(const terminator& t);
        virtual ~terminator();

		ull_t get_count_of_terminated_requests() const  //!< ���������� ���������� ��������������� ��������
		{ return count_of_terminated_requests; }

        void terminate(request* req);					//!< �������, ������������� ������
		virtual void add(request* req);                 //!< ���������� ����������� ������� �������� ������ object
		virtual request* get_request();					//!< ���������� ����������� ������� �������� ������ object                        

    private:
        int terminating_period;
        ull_t count_of_terminated_requests;
    };

} //end namespace logic

#endif // TERMINATOR_H
