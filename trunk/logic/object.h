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
    class model;

	//! Базовый абстрактный класс для элемента модели

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

		bool is_moveable() const				//!< Проверяет, можно ли из входного объекта "вытащить" запрос
		{ return moveable_request_flag; }

		bool is_free() const					//!< Проверяет, можно ли в текущего объект добавить запрос
		{ return freedom_flag; }

		void connect_with(object* _source);		//!< Служит для соединения объекта с входом
		bool has_connection() const;			//!< Проверка наличия соединения
		object* connected_with() const 
		{ return input; }
		void set_parrent(model* parent);		//!< Устанавливает указателя на родительскую модель

		virtual request* get_request() = 0;		//!< "Вытаскивает" запрос из входа
		virtual void add(request* req) = 0;		//!< Добавление запроса в текущий объект
        virtual void move_request();		    //!< Служит для вызова виртуальной функции add
		
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
