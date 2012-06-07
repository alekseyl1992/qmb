#ifndef HANDLER_H
#define HANDLER_H

#include "object.h"
#include "request.h"

namespace logic
{
    class model;

	//! ����� ����������� ���������. �������� ��������� logic::model.
    /*!
     * ������������ ����� ������, ����������� ��� ��������� ��������� (��������) �
     * ������ logic::model. ��������� ���������� �� ����������� ������� (�������).
     */

    class handler : public object
	{
	public:
        handler(ull_t id = 0, int _handlePeriod = 0);
		handler(const handler& h);
        virtual ~handler();

        ull_t get_num_of_handled_requests() const     //!< ���������� ���������� ������������ ��������
		{ return count_of_handled_requests; }  

        void handle(request* req);					  //!< �������, �������������� ������
		virtual void add(request* req);               //!< ���������� ����������� ������� �������� ������ object
        virtual request* get_request();				  //!< ���������� ����������� ������� �������� ������ object

	private:
		int handling_period;
        ull_t count_of_handled_requests;
	};

} //end namespace logic

#endif // !HANDLER_H
