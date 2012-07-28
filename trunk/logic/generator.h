#ifndef GENERATOR_H
#define GENERATOR_H

#include "object.h"
#include "request.h"
#include <QString>
#include <QtScript>

namespace logic
{
    class model;

    //! Класс генератора. Является элементом модели (класс logic::model).
    /*!
     * Представляет собой объект, испольуемый для генерации сообщений (запросов) в
     * модели logic::model. Является абсолютно независимым элементом СМО, не имеет входа.
     * Генерация происходит по различным законам распределения.
     */

    class generator : public object
    {
        //Q_OBJECT

    public:
        generator(std::string name, int id, QString script, int period = 0, ull_t num_requests = 0, bool is_random = false, bool is_infinite = false);
        generator(const generator& gen);
        virtual ~generator();

        ull_t get_num_requests() const 													//!< Возвращает количество запросов, которое требуется сгенерировать
        { return number_of_requests_to_generate; }

        ull_t get_current_num_requests() const											//!< Возвращает текущее количество сгенерированных запросов
        { return count_of_generated_requests; }

        bool is_infinite() const													    //!< Возвращает флаг бесконечной генерации запросов
        { return infinite_generating; }

        void generate_new_request(ull_t r_id);											//!< Генерирует новый запрос
        void generating();
        virtual request* get_request();													//!< Реализация виртуальной функции базового класса object
        virtual void add(request*) { }                                                  //!< Реализация виртуальной функции базового класса object
        virtual void move_request();

        virtual bool is_completed()
        { return count_of_generated_requests == number_of_requests_to_generate; }

    private:
        QString script;
        QScriptEngine engine;
        int generating_period;
        ull_t number_of_requests_to_generate;
        bool random_generating;
        bool infinite_generating;

        ull_t count_of_generated_requests;
    };

} //end namespace logic

#endif // !GENERATOR_H
