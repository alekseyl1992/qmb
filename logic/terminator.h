#ifndef TERMINATOR_H
#define TERMINATOR_H

#include "object.h"
#include "request.h"

namespace logic
{
    class model;

	//! Класс терминатора. Является элементом logic::model.
    /*!
     * Представляет собой объект, испольуемый для удаления сообщений (запросов) из
     * модели logic::model. Завершают "жизнь" сообщения в модели.
     */

    class terminator : public object
    {
    public:
        terminator(ull_t id = 0, int period = 0);
        terminator(const terminator& t);
        virtual ~terminator();

		ull_t get_count_of_terminated_requests() const  //!< Возвращает количество терминированных запросов
		{ return count_of_terminated_requests; }

        void terminate(request* req);					//!< Функция, терминирующая запрос
		virtual void add(request* req);                 //!< Реализация виртуальной функции базового класса object
		virtual request* get_request();					//!< Реализация виртуальной функции базового класса object                        

    private:
        int terminating_period;
        ull_t count_of_terminated_requests;
    };

} //end namespace logic

#endif // TERMINATOR_H
