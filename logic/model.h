#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <list>
#include <string>
#include <utility>
#include <algorithm>
#include <thread>
#include <mutex>
#include <sstream>
#include <QObject>

#include "generator.h"
#include "queue.h"
#include "handler.h"
#include "terminator.h"
#include "collector.h"
#include "separator.h"
#include "attribute.h"
#include "exceptions.h"


//! Перечисление ошибок модели
enum error_code {
    INPUT_IN_GENERATOR,
    OUTPUT_IN_TERMINATOR,
    NO_GENERATORS,
    NO_TERMINATORS,
    N
};

namespace logic
{
    //! Класс модель.
    /*!
     * Представляет собой объект, который содержит списки объектов и реализует все связи между ними.
     */

    class model : public QObject
    {
        Q_OBJECT

        friend class object;
        /*friend class generator;
        friend class queue;
        friend class handler;
        friend class terminator;
        friend class collector;
        friend class separator;*/

        typedef std::pair<object*, error_code> Pair;
    public:
        model();
        model(const model& m);
        ~model();

    private:
        bool is_simulating_finished();								//!< Проверяет, завершена ли симуляция

        void generating_th();										//!< Функция, создающая потоки для генерации сообщений
        void new_thread(object* obj);
        void threading();											//!< Функция, создающая потоки для перемещения запросов по модели
        void checking_finished_th();								//!< Функция, проверяющая систему на завершенность

        void try_pausing() const;									//!< Действия, связанные с введением модели в состояние паузы

    public:
        void add_object(object* obj);
        object* find_object(int global_id);

        void connect(object* lhs, object* rhs);						//!< Соединяет два элемента модели

        void add_attribute(attribute& attr);                        //!< Добавляет новый атрибут в модель
        void remove_attribute(int id);                              //!< Удаляет атрибут из модели по id
        attribute& get_attribute(int id);                           //!< Возвращает атрибут по id

        bool ExitPointSearch(object* obj);
        bool HasExitPoint(object* obj);
        std::string intToString(int val);
        bool is_valid();											//!< Проверяет модель на наличие ошибок
        std::vector<Pair> get_errors() const						//!< Возвращает ошибки модели
        { return errors; }

        bool is_simulating() const									//!< Показывает состояние симуляции (не то же, что is_simulating_finished())
        { return simulate_flag && !stop_flag; }

        bool is_paused() const 										//!< Если модель стоит на паузе
        { return pause_flag; }

        ull_t get_start_time() const
        { return start_time; }

        void simulation_start();									//!< Начинает симуляцию
        void simulation_stop();										//!< Останавливает симуляцию
        void simulation_pause();									//!< Ставит симуляцию на паузу
        void simulation_restore();                                  //!< Восстанавливает симуляцию с паузы

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
        std::list<object*> generators;
        std::list<object*> queues;
        std::list<object*> handlers;
        std::list<object*> terminators;
        std::list<object*> collectors;
        std::list<object*> separators;
        std::vector<object*> objects;

        std::vector<Pair> errors;
        std::vector<std::thread*> threads;

        attributes_list attributes;

        bool simulate_flag;
        bool stop_flag;
        bool pause_flag;
        ull_t start_time;

        ull_t num_terminated;
        ull_t num_generated;
    };

} //end namespace logic

#endif // !MODEL_H
