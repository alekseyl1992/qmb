#include "model.h"

namespace qmodel
{
    int model::cur_id = 0;

    bool model::is_all_generated() {
		bool all_completed = true;
		std::for_each(link_generators_queues.begin(), link_generators_queues.end(), [&all_completed](link <generator*, queue*>& link) {
			if (!link.lhs->is_finished())
			{
				all_completed = false;
			}
		});
		return all_completed;
	}

	bool model::is_queues_clear() {
		bool all_completed = true;
		std::for_each(link_queues_handlers.begin(), link_queues_handlers.end(), [&all_completed](link <queue*, handler*>& link) {
			if (link.lhs->get_size() != 0)
			{
				all_completed = false;
			}
		});
		return all_completed;
	}

	bool model::is_simulating() {
		return !(is_all_generated() && is_queues_clear());
	}

	void model::generator_queue_link_th() {
		std::for_each(link_generators_queues.begin(), link_generators_queues.end(), 
			[&](link<generator*, queue*>& link) 
		{
			std::thread([&link, this]()
			{
                for(int i = 1; i <= link.lhs->get_num_requests(); i++)
				{
                    if (!simulate_flag || !is_simulating()) //if the user switched simulating off
                    {
						break;
                    }
					link.lhs->generate_new_request();
					link.rhs->add(link.lhs->get_request());
				}
			}).detach();
		});
	}

	void model::handler_queue_link_th() {
		std::for_each(link_queues_handlers.begin(), link_queues_handlers.end(), 
				[&](link<queue*, handler*>& link) 
		{
			std::thread([&link, this]()
			{
                for(; ; )
				{
                    if (!simulate_flag || !is_simulating()) //if the user switched simulating off
                    {
                        emit simulationFinished();
                        break;
                    }
					std::unique_lock<std::mutex> lk(model_mutex);
					if (link.lhs->has_request() && link.rhs->is_free())
					{
						link.rhs->handle(link.lhs->get_first());
					}
				}
			}).detach();
		});
	}

	void model::simulation_start() {
		//starts simulating
		simulate_flag = true;
		generator_queue_link_th();
		handler_queue_link_th();
	}

	void model::simulation_stop() {
		//stops simulating
		simulate_flag = false;
	}

} //end namespace qmodel
