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
	//��� �����, ������� ��������� ��� ����� ����� ��������� ������
	//�� �� �������� ������ ��������, ��������� ����������� � �.�. � �.�.
	//��� ������� ������ ������
    //� �������� �� ModelStorage

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

		virtual void clean() { }

		void simulation_start();
		void simulation_stop();

    signals:
        void simulationFinished();
        void reqGenerated(const request_id& reqID);
        void reqQueued(const int& qID, const request_id& reqID);
        void reqBeganHandling(const int& hID, const request_id& reqID);
        void reqFinishedHandling(const int& hID, const request_id& reqID);

	private:
		void generator_queue_link_th();
		void handler_queue_link_th();


	public: //members <- TODO: make private
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
