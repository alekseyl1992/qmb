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


//! ������������ ������ ������
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
    //! ����� ������.
    /*!
     * ������������ ����� ������, ������� �������� ������ �������� � ��������� ��� ����� ����� ����.
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
        bool are_all_generated();									//!< ���������, ��������� �� ��� ���������� ������
        bool are_all_queues_clear();								//!< ���������, ��� �� � �������� ��������
        bool are_all_handlers_finished_handling();					//!< ���������, ��������� �� ��� ����������� ���� ������
        bool are_all_terminators_finished_terminating();			//!< ���������, ��������� �� ��� ��������� ���� ������
        bool is_simulating_finished();								//!< ���������, ��������� �� ���������

        void generating_th();										//!< �������, ��������� ������ ��� ��������� ���������
		void threading();											//!< �������, ��������� ������ ��� ����������� �������� �� ������
        void checking_finished_th();								//!< �������, ����������� ������� �� �������������

        void try_pausing() const;									//!< ��������, ��������� � ��������� ������ � ��������� �����

    public:
		void add_generator(generator &&gen);						//!< ��������� ��������� � ������
        void add_queue(queue &&q);									//!< ��������� ������� � ������
        void add_handler(handler &&h);								//!< ��������� ���������� � ������
        void add_terminator(terminator &&t);						//!< ��������� ���������� � ������

		void connect(object* lhs, object* rhs);						//!< ��������� ��� �������� ������

        object* find_object(ull_t global_id);
        generator* find_generator(ull_t id);						//!< ���������� ����� ���������� � ������ id
        queue* find_queue(ull_t id);								//!< ���������� ����� ������� � ������ id
        handler* find_handler(ull_t id);							//!< ���������� ����� ����������� � ������ id
        terminator* find_terminator(ull_t id);						//!< ���������� ����� ����������� � ������ id

		bool is_valid();											//!< ��������� ������ �� ������� ������
		std::string get_errors() const;								//!< ���������� ������ ������

		bool is_simulating() const									//!< ���������� ��������� ��������� (�� �� ��, ��� is_simulating_finished())
		{ return simulate_flag && !stop_flag; }

		bool is_paused() const 										//!< ���� ������ ����� �� �����
		{ return pause_flag; }

        ull_t get_start_time() const
        { return start_time; }

        void simulation_start();									//!< �������� ���������
        void simulation_stop();										//!< ������������� ���������
		void simulation_pause();									//!< ������ ��������� �� �����

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
