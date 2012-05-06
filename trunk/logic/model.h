#ifndef H_MODEL
#define H_MODEL

#include <vector>
#include <algorithm>
#include <thread>
#include <mutex>
#include <sstream>
#include <QObject>

#include "request.h"
#include "generator.h"
#include "queue.h"
#include "handler.h"
#include "link.h"
#include "exceptions.h"

namespace logic
{
	//это класс, который реализует все связи между объектами модели
	//он же содержит списки очередей, связанных генераторов и т.д. и т.п.
	//это входной вектор логики
    //и выходной из ModelStorage

    class model : public QObject
	{
        Q_OBJECT

        std::mutex model_mutex;
        friend class generator;
        friend class queue;
        friend class handler;

	public:
        model() { }
		model(const model& ) { }

		~model() { }

        bool are_all_generated(); //checks if all generators finished their work
        bool are_queues_clear(); //checks if all queues are clear
        bool are_all_handlers_finished_handling(); //it's obvious :)
        bool is_simulating_finished(); //checks the sumulation
        void generator_queue_link_th(); //links generators and queues
        void handler_queue_link_th(); //links queues and handlers

        void simulation_start();
		void simulation_stop();

        void add_generator(const generator &&gen);
        void add_queue(const queue &&q);
        void add_handler(const handler &&h);
        void add_link_generator_queue(const link <generator*, queue*> &&link);
        void add_link_queue_handler(const link<queue *, handler *> &&link);

        generator* get_generator_by_id(int id);
        queue* get_queue_by_id(int id);
        handler* get_handler_by_id(int id);
        link <generator*, queue*>* get_link_generator_queue_by_ids(int id_left, int id_right);
        link <queue*, handler*>* get_link_queues_handlers_by_ids(int id_left, int id_right);

    signals:
        void simulationFinished();
        void reqGenerated(const request_id& reqID);
        void reqQueued(const int& qID, const request_id& reqID);
        void reqBeganHandling(const int& hID, const request_id& reqID);
        void reqFinishedHandling(const int& hID, const request_id& reqID);

    private: //members
		std::vector<generator> generators; //all generators are kept here
		std::vector<queue> queues; //all queues are kept here
		std::vector<handler> handlers; //all handlers are kept here

		//links
		std::vector< link <generator*, queue*> > link_generators_queues;
		std::vector< link <queue*, handler*> > link_queues_handlers;

		bool simulate_flag;
	};

} //end namespace logic

#endif // !H_MODEL
