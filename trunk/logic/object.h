#ifndef OBJECT_H
#define OBJECT_H

#include <thread>
#include <mutex>
#include <string>
#include <sstream>
#include <iostream>
#include <QObject>

#include "../utility/common.h"
#include "request.h"

namespace logic
{
    ull_t get_now_time()
    {
        auto now = std::chrono::system_clock::now();
        auto duration  = now.time_since_epoch();
        return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
    }

    class model;

	//! ������� ����������� ����� ��� �������� ������

    class object : public QObject
	{
        Q_OBJECT

	public:
		object(ItemType type = ItemType::NoType, ull_t id = 0);
        object(const object& obj);
        virtual ~object();

		ItemType get_type() const  
		{ return item_type; }

        ull_t get_id() const 
		{ return id; }

		bool is_moveable() const				//!< ���������, ����� �� �� �������� ������� "��������" ������
		{ return moveable_request_flag; }

		bool is_free() const					//!< ���������, ����� �� � �������� ������ �������� ������
		{ return freedom_flag; }

		void connect_with(object* _source);		//!< ������ ��� ���������� ������� � ������
		bool has_connection() const;			//!< �������� ������� ����������
		object* connected_with() const 
		{ return input; }
		void set_parrent(model* parent);		//!< ������������� ��������� �� ������������ ������

		virtual request* get_request() = 0;		//!< "�����������" ������ �� �����
		virtual void add(request* req) = 0;		//!< ���������� ������� � ������� ������
		virtual void move_the_request();		//!< ������ ��� ������ ����������� ������� add
		
	protected:
		ItemType item_type;
        model* parent;
        ull_t id;
		object* input;
		request* cur_req;
		
		bool moveable_request_flag;
		bool freedom_flag;
		std::mutex item_mutex;

        //attributes
        std::string name;
	};

} //end namespace logic

#endif // !OBJECT_H
