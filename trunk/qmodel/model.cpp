#include "model.h"

namespace qmodel
{
	/**********************Implementation**********************/

	int model::cur_id = 0;

	void model::generator_queue_link_th() {
		std::for_each(link_generators_queues.begin(), link_generators_queues.end(), 
			[&](const link<generator*, queue*>& link) 
		{
			std::thread([&link, this]()
			{
				for(int i = 1; i<=10 && is_simulating(); i++)
				{
					link.lhs->generate_new_request();
					link.rhs->add(link.lhs->get_request());
				}
			}).detach();

		});
	}

	void model::handler_queue_link_th() {
		std::for_each(link_queues_handlers.begin(), link_queues_handlers.end(), 
				[&](const link<queue*, handler*>& link) 
		{
			std::thread([&link, this]()
			{
				for(; is_simulating();)
				{
                    std::lock_guard<std::mutex> lock(model_mutex);
					if (link.lhs->has_request() && link.rhs->is_free())
					{
						link.rhs->handle(link.lhs->get_first());
					}
				}
			}).detach();

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
