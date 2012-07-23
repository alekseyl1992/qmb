#ifndef HANDLER_H
#define HANDLER_H

#include "object.h"
#include "request.h"

namespace logic
{
    class model;

    //! Класс обработчика сообщений. Является элементом logic::model.
    /*!
     * Представляет собой объект, испольуемый для обработки сообщений (запросов) в
     * модели logic::model. Обработка происходит по абсолютному времени (периоду).
     */

    class handler : public object
    {
    public:
        handler(std::string name, int id = 0, int _handlePeriod = 0);
        handler(const handler& h);
        virtual ~handler();

        ull_t get_num_of_handled_requests() const     //!< Возвращает количество обработанных запросов
        { return count_of_handled_requests; }

        virtual void add(request* req);               //!< Реализация виртуальной функции базового класса object
        virtual request* get_request();				  //!< Реализация виртуальной функции базового класса object

    private:
        void handle(request* req);					  //!< Функция, обрабатывающая запрос

    private:
        int handling_period;
        ull_t count_of_handled_requests;
    };

} //end namespace logic

#endif // !HANDLER_H
