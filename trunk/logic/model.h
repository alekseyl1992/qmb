#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <list>
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
#include "exceptions.h"


//! Перечисление ошибок модели
enum error_code {
    INPUT_IN_GENERATOR,
    OUTPUT_IN_TERMINATOR,
    NO_GENERATORS,
    NO_TERMINATORS,
    N
};

std::string error_code_str(error_code code);

namespace logic
{
    //! Класс модель.
    /*!
     * Представляет собой объект, который содержит списки объектов и реализует все связи между ними.
     */

    class model : public QObject
	{
        Q_OBJECT

        friend class generator;
        friend class queue;
        friend class handler;
        friend class terminator;

	public:
        model();
		model(const model& m);
		~model();

        //static std::vector< std::pair<ItemType, ItemType> > supportedLinks();
		
    private:
        bool are_all_generated();									//!< Проверяет, завершили ли все генераторы работу
        bool are_all_queues_clear();								//!< Проверяет, нет ли в очередях запросов
        bool are_all_handlers_finished_handling();					//!< Проверяет, закончили ли все обработчики свою работу
        bool are_all_terminators_finished_terminating();			//!< Проверяет, закончили ли все термиторы свою работу
        bool is_simulating_finished();								//!< Проверяет, завершена ли симуляция

        void generating_th();										//!< Функция, создающая потоки для генерации сообщений
		void threading();											//!< Функция, создающая потоки для перемещения запросов по модели
        void checking_finished_th();								//!< Функция, проверяющая систему на завершенность

        void try_pausing() const;									//!< Действия, связанные с введением модели в состояние паузы

    public:
		void add_generator(generator &&gen);						//!< Добавляет генератор в модель
        void add_queue(queue &&q);									//!< Добавляет очередь в модель
        void add_handler(handler &&h);								//!< Добавляет обработчик в модель
        void add_terminator(terminator &&t);						//!< Добавляет терминатор в модель

		void connect(object* lhs, object* rhs);						//!< Соединяет два элемента модели

        object* find_object(ull_t global_id);
        generator* find_generator(ull_t id);						//!< Возвращает адрес генератора с нужным id
        queue* find_queue(ull_t id);								//!< Возвращает адрес очереди с нужным id
        handler* find_handler(ull_t id);							//!< Возвращает адрес обработчика с нужным id
        terminator* find_terminator(ull_t id);						//!< Возвращает адрес терминатора с нужным id

		bool is_valid();											//!< Проверяет модель на наличие ошибок
		std::string get_errors() const;								//!< Возвращает ошибки модели

		bool is_simulating() const									//!< Показывает состояние симуляции (не то же, что is_simulating_finished())
		{ return simulate_flag && !stop_flag; }

		bool is_paused() const 										//!< Если модель стоит на паузе
		{ return pause_flag; }

        ull_t get_start_time() const
        { return start_time; }

        void simulation_start();									//!< Начинает симуляцию
        void simulation_stop();										//!< Останавливает симуляцию
		void simulation_pause();									//!< Ставит симуляцию на паузу

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

    private: //members
        typedef std::pair<object*, error_code> Pair;

		std::list<generator> generators;
		std::list<queue> queues;
		std::list<handler> handlers;
        std::list<terminator> terminators;

        std::vector<object*> objects;

		std::vector<Pair> errors;
		std::vector<std::thread*> threads;

		bool simulate_flag;
		bool stop_flag;
		bool pause_flag;
        ull_t start_time;

		ull_t num_terminated;
		ull_t num_generated;
	};

} //end namespace logic

#endif // !MODEL_H
