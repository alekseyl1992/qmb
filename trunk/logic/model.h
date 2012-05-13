#ifndef H_MODEL
#define H_MODEL

#include <vector>
#include <algorithm>
#include <thread>
#include <mutex>
#include <sstream>
#include <vector>
#include <QObject>
#include <ctime>

#include "utility/common.h"
#include "request.h"
#include "generator.h"
#include "queue.h"
#include "handler.h"
#include "terminator.h"
#include "link.h"
#include "exceptions.h"

namespace logic
{
    //это класс, который содержит списки объектов и реализует все связи между объектами модели

    class model : public QObject
	{
        Q_OBJECT

        friend class generator;
        friend class queue;
        friend class handler;
        friend class terminator;

        std::mutex model_mutex, model_mutex2, model_mutex3;
	public:
        model() { }
		model(const model& ) { }
        static std::vector< std::pair<ItemType, ItemType> > supportedLinks();

		~model() { }

    private:
        bool are_all_generated(); //checks if all generators finished their work
        bool are_all_queues_clear(); //it's obvious :)
        bool are_all_handlers_finished_handling();
        bool are_all_terminators_finished_terminating();
        bool is_simulating_finished(); //checks the sumulation
        void generating_th(); //links generators and queues
        void queueing_th();
        void queue_handler_link_th(); //links queues and handlers
        void handler_terminator_link_th(); //links queues and handlers

    public:
        void simulation_start();
        int simulation_stop();

        void add_generator(const generator &&gen);
        void add_queue(const queue &&q);
        void add_handler(const handler &&h);
        void add_terminator(const terminator &&t);
        void add_link_generator_queue(const link <generator *, queue *> &&link);
        void add_link_queue_handler(const link<queue *, handler *> &&link);
        void add_link_handler_terminator(const link<handler *, terminator *> &&link);

        generator* get_generator_by_id(int id);
        queue* get_queue_by_id(int id);
        handler* get_handler_by_id(int id);
        terminator* get_terminator_by_id(int id);

        link <generator*, queue*>* get_link_generator_queue_by_ids(int id_left, int id_right);
        link <queue*, handler*>* get_link_queues_handlers_by_ids(int id_left, int id_right);

    signals:
        void simulationFinished(int time);
        void reqGenerated(const logic::request_id& reqID, int time);
        void reqQueued(const int& qID, const logic::request_id& reqID, int time);
        void reqBeganHandling(const int& hID, const logic::request_id& reqID, int time);
        void reqFinishedHandling(const int& hID, const logic::request_id& reqID, int time);
        void reqTerminated(const int& tID, const logic::request_id& reqID, int time);

    private: //members
		std::vector<generator> generators; //all generators are kept here
		std::vector<queue> queues; //all queues are kept here
		std::vector<handler> handlers; //all handlers are kept here
        std::vector<terminator> terminators; //all terminators are kept here

		//links
		std::vector< link <generator*, queue*> > link_generators_queues;
		std::vector< link <queue*, handler*> > link_queues_handlers;
        std::vector< link <handler*, terminator*> > link_handlers_terminators;

		bool simulate_flag;
        ull_t start_time;
	};

} //end namespace logic

#endif // !H_MODEL
