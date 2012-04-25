#include "model.h"

namespace qmodel
{
    int model::cur_id = 0;

	void model::generator_queue_link_th() {
		std::for_each(link_generators_queues.begin(), link_generators_queues.end(), 
			[&](link<generator*, queue*>& link) 
		{
			std::thread th([&link, this]()
			{
				for(int i = 1; i <= link.lhs->get_num_requests() && is_simulating(); i++)
				{
					link.lhs->generate_new_request();
					link.rhs->add(link.lhs->get_request());

					if (!simulate_flag)
						break;
				}
			});
			//threads.push_back(&th);
			th.detach();
		});
	}

	void model::handler_queue_link_th() {
		std::for_each(link_queues_handlers.begin(), link_queues_handlers.end(), 
				[&](link<queue*, handler*>& link) 
		{
			std::thread th([&link, this]()
			{
				for(; is_simulating(); )
				{
					if (!simulate_flag)
						break;
					std::unique_lock<std::mutex> lk(model_mutex);
					if (link.lhs->has_request() && link.rhs->is_free())
					{
						//lk.unlock();
						link.rhs->handle(link.lhs->get_first());
					}

					
				}
			});
			//threads.push_back(&th);
			th.detach();
		});
	}

	void model::simulation_start() {
		simulate_flag = true;
		generator_queue_link_th();
		handler_queue_link_th();
	}

	void model::simulation_stop() {
		//останавливает выполнение
		simulate_flag = false;
	}

} //end namespace qmodel
