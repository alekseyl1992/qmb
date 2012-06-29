﻿#ifndef OBJECT_H
#define OBJECT_H

#include <thread>
#include <mutex>
#include <string>
#include <sstream>
#include <iostream>
#include <QObject>
#include <QDebug>

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

        ull_t get_global_id() const
        { return global_id; }

		bool is_moveable() const				//!< Проверяет, можно ли из входного объекта "вытащить" запрос
		{ return moveable_request_flag; }

		bool is_free() const					//!< Проверяет, можно ли в текущего объект добавить запрос
		{ return freedom_flag; }

        virtual void set_input(object* _source);		//!< Служит для соединения объекта с входом
        virtual void set_output(object* _dest);         //!< Служит для соединения объекта с выходом

        virtual bool has_input() const;
        virtual bool has_output() const;

        object* input_connection() const    //ex connected_with()
        { return input; }

        object* output_connection() const
        { return output; }

        void set_parrent(model* parent);		//!< Устанавливает указатель на родительскую модель


		virtual request* get_request() = 0;		//!< "Вытаскивает" запрос из входа
		virtual void add(request* req) = 0;		//!< Добавление запроса в текущий объект
        virtual void move_request();            //!< Служит для вызова виртуальной функции add
        virtual bool is_completed() = 0;        //!< Служит для проверки объекта на завершенность работы

        int get_event_time() const;

	protected:
		ItemType item_type;
        model* parent;
        ull_t id;
        ull_t global_id;
        object* input;
        object* output;
		request* cur_req;
		
		bool moveable_request_flag;
		bool freedom_flag;
		std::mutex item_mutex;

        //attributes
        std::string name;
	};

} //end namespace logic

#endif // !OBJECT_H
