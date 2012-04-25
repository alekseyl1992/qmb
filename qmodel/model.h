#ifndef H_MODEL
#define H_MODEL

#include <vector>
#include <algorithm>
#include <thread>
#include <mutex>

#include "request.h"
#include "generator.h"
#include "queue.h"
#include "handler.h"
#include "link.h"
#include "exceptions.h"
#include "object.h"

namespace qmodel
{
	//это класс, который реализует все связи между объектами модели
	//он же содержит списки очередей, связанных генераторов и т.д. и т.п.
	//это входной вектор логики
	//и выходной из Converter
	
    class model : public object
	{
		
	public:
		std::mutex model_mutex;
        model(): object(++cur_id) { }
		model(const model& ) { }

		~model()
		{
		}

		std::vector<generator> generators;
		std::vector<queue> queues;
		std::vector<handler> handlers;

		//links
		std::vector< link <generator*, queue*> > link_generators_queues;
		std::vector< link <queue*, handler*> > link_queues_handlers;

		bool is_all_generated() {
			bool all_completed = true;
			std::for_each(link_generators_queues.begin(), link_generators_queues.end(), [&all_completed](link <generator*, queue*>& link) {
				if (!link.lhs->is_finished())
				{
					all_completed = false;
				}
			});
			return all_completed;
		}

		bool is_queue_clear() {
			bool all_completed = true;
			std::for_each(link_queues_handlers.begin(), link_queues_handlers.end(), [&all_completed](link <queue*, handler*>& link) {
				if (link.lhs->get_size() != 0)
				{
					all_completed = false;
				}
			});
			return all_completed;
		}

		bool is_simulating() {
			bool ag = is_all_generated();
			bool qc = is_queue_clear();
			return !(is_all_generated() && is_queue_clear());
		}

		//algorithm
		public:
			void simulation_start();
			void simulation_stop();
		private:
			void generator_queue_link_th();
			void handler_queue_link_th();

			
		std::vector<std::thread*> threads;

		virtual void clean() { }

	private:
		bool simulate_flag;
		static int cur_id;
	};

} //end namespace qmodel

#endif // !H_MODEL
