#ifndef QUEUE_H
#define QUEUE_H

#include <deque>

#include "object.h"
#include "request.h"

namespace logic
{
    class model;
	
    //! Класс очереди. Является элементом logic::model.
    /*!
     * Представляет собой объект, испольуемый для хранения сообщений (запросов)
     * модели logic::model и дальнейшей передачи сообщений в другие элемнты модели.
     */

    class queue : public object
    {
    public:
        queue(int id = 0, bool from_top = true);
        queue(const queue& q);
        virtual ~queue();

        ull_t get_size() const                                    //!< Возвращает размер очереди
        { return static_cast<ull_t>(requests.size()); }

    private:
        void make_cur_request();                                  //!< Создает текущий запрос согласно начальным условиям

    public:
        virtual void add(request* req);                           //!< Реализация виртуальной функции базового класса object
        request* get_first();                                     //!< Возвращает верхний элемент списка
        request* get_last();									  //!< Возвращает нижний элемент списка
        virtual request* get_request();                           //!< Реализация виртуальной функции базового класса object

        virtual bool is_completed()
        { return get_size() == 0; }

    private:
        std::deque<request*> requests;
        bool to_get_from_top;
    };

} //end namespace logic

#endif // !QUEUE_H
