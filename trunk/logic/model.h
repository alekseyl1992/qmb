#ifndef MODEL_H
#define MODEL_H

#include <list>
#include <iostream>
#include <string>
#include <algorithm>
#include <thread>
#include <QObject>
#include <QtScript>

#include "object.h"
#include "attribute.h"
#include "exceptions.h"

namespace logic
{
    //! Класс модель.
    /*!
     * Представляет собой объект, который содержит списки логических объектов и реализует все связи между ними.
     */

    class model : public QObject
    {
        Q_OBJECT

        friend class object;
        friend class generator;
        friend class queue;
        friend class handler;
        friend class terminator;
        //friend class collector;
        //friend class separator;

        enum simulationCondition
        {
            Start,
            Stop,
            Pause,
            Restore
        };

    public:
        model();
        ~model();

    private:
        bool is_simulating_finished();								//!< Проверяет, завершена ли симуляция
        bool is_simulating() const;									//!< Показывает состояние симуляции (не то же, что is_simulating_finished())

        void try_pausing() const;									//!< Действия, связанные с введением модели в состояние паузы
        bool is_paused() const; 									//!< Проверяет модель на паузу

        ull_t get_start_time() const;                               //!< Функция, возвращающая время начала симуляции
        void changeCondition(simulationCondition cond);

        void generating_th();										//!< Функция, создающая потоки для генерации сообщений
        bool thread_necessary(object* obj);                         //!< Проверяет, необходим ли поток для данного объекта
        void new_thread(object* obj);                               //!< Функция, создающий новый поток
        void threading();											//!< Функция, создающая потоки для перемещения запросов по модели
        void checking_finished_th();								//!< Функция, проверяющая систему на завершенность

        bool ExitPointSearch(object* obj);                          //!< Функция, проверяющая налисие "выходной точки" у объекта
        bool HasExitPoint(object* obj);                             //!< Функция, вызывающая ExitPointSearch()

    public:
        bool is_valid();											//!< Проверяет модель на наличие ошибок

        void simulation_start();									//!< Начинает симуляцию
        void simulation_stop();										//!< Останавливает симуляцию
        void simulation_pause();									//!< Ставит симуляцию на паузу
        void simulation_restore();                                  //!< Восстанавливает симуляцию с паузы

        void add_object(object* obj);                               //!< Добавляет объект в модель
        object* find_object(int global_id);                         //!< Возвращает объект по глобальному id
        void connect(object* lhs, object* rhs);						//!< Соединяет два элемента модели

        void add_attribute(attribute& attr);                        //!< Добавляет новый атрибут в модель
        void remove_attribute(int id);                              //!< Удаляет атрибут из модели по id
        attribute& get_attribute(int id);                           //!< Возвращает атрибут по id
        std::exception_ptr get_stop_reason() const                  //!< Вовращает причину остановки или пустой объект в случае отсутсвия оной
        { return stop_reason; }

    signals:
        void simulationStarted(int time);
        void simulationStopped(int time);
        void simulationPaused(int time);
        void simulationRestored(int time);
        void simulationFinished(int time);
        void reqGenerated(const logic::request_id& reqID, int time);
        void reqQueued(const int& qID, const logic::request_id& reqID, int time);
        void reqBeganHandling(const int& hID, const logic::request_id& reqID, int time);
        void reqFinishedHandling(const int& hID, const logic::request_id& reqID, int time);
        void reqTerminated(const int& tID, const logic::request_id& reqID, int time);

    private:
        std::list<object*> objects;
        std::list<object*> generators;
        std::list<object*> queues;
        std::list<object*> handlers;
        std::list<object*> terminators;
        std::list<object*> collectors;
        std::list<object*> separators;

        std::exception_ptr stop_reason;

        std::vector<std::thread*> threads;

        attributes_list attributes;

        bool simulate_flag;
        bool stop_flag;
        bool pause_flag;
        ull_t start_time;
    };

} //end namespace logic

#endif // !MODEL_H
