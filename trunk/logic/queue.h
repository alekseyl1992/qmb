#ifndef QUEUE_H
#define QUEUE_H

#include <deque>

#include "object.h"
#include "request.h"

namespace logic
{
    class model;
	
	//! ����� �������. �������� ��������� logic::model.
    /*!
     * ������������ ����� ������, ����������� ��� �������� ��������� (��������)
     * ������ logic::model � ���������� �������� ��������� � ������ ������� ������.
     */

    class queue : public object
	{
    public:
        queue(int id = 0, bool from_top = true);
        queue(const queue& q);
		virtual ~queue();
		
        ull_t get_size() const                                    //!< ���������� ������ �������
		{ return static_cast<ull_t>(requests.size()); }

	private:
		void make_cur_request();                                  //!< ������� ������� ������ �������� ��������� ��������

	public:
		virtual void add(request* req);                           //!< ���������� ����������� ������� �������� ������ object
		request* get_first();                                     //!< ���������� ������� ������� ������
		request* get_last();									  //!< ���������� ������ ������� ������
		virtual request* get_request();                           //!< ���������� ����������� ������� �������� ������ object

	private:
		std::deque<request*> requests;
		bool to_get_from_top;
	};

} //end namespace logic

#endif // !QUEUE_H
