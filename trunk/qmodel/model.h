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
#include "qmbObject.h"

namespace qmodel
{
	//это класс, который реализует все связи между объектами модели
	//он же содержит списки очередей, связанных генераторов и т.д. и т.п.
	//это входной вектор логики
	//и выходной из Converter
	
	class model : public qmbObject
	{
		std::mutex model_mutex;
	public:
		model(): qmbObject(++cur_id) { }
		model(const model& ) { }

		std::vector<generator> generators;
		std::vector<queue> queues;
		std::vector<handler> handlers;

		//links
		std::vector< link <generator*, queue*> > link_generators_queues;
		std::vector< link <queue*, handler*> > link_queues_handlers;

		//algorithm
		public:
			void simulation_start();
			void simulation_stop();
		private:
			void generator_queue_link_th();
			void handler_queue_link_th();

			bool is_simulating() {
                //std::lock_guard<std::mutex> lock(model_mutex);
				return simulate_flag;
			}
		
	private:
		bool simulate_flag;

		static int cur_id;
	};
	
} //end namespace qmodel  

#endif // !H_MODEL
