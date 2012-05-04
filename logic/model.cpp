#include "model.h"

namespace logic
{
    bool model::are_all_generated() {
        bool all_completed = true;
       /* for (link <generator*, queue*>& link : link_generators_queues)
        {
            if (!link.lhs->is_finished())
            {
                all_completed = false;
                break;
            }
        }*/

        std::for_each(link_generators_queues.begin(), link_generators_queues.end(), [&all_completed](link <generator*, queue*>& link) {
			if (!link.lhs->is_finished())
			{
                all_completed = false;
			}
        });
        return all_completed;
	}

    bool model::are_queues_clear() {
        bool all_completed = true;
        std::for_each(link_queues_handlers.begin(), link_queues_handlers.end(), [&all_completed](link <queue*, handler*>& link) {
			if (link.lhs->get_size() != 0)
			{
                all_completed = false;
			}
        });
    return all_completed;
    }

    bool model::are_all_handlers_finished_handling()
    {
        bool all_completed = true;
        std::for_each(link_queues_handlers.begin(), link_queues_handlers.end(), [&all_completed](link <queue*, handler*>& link) {
            if (!link.rhs->is_free())
            {
                all_completed = false;
            }
        });
        return all_completed;
    }



    bool model::is_simulating_finished() {
        bool c1 = are_all_generated(),
             c2 = are_queues_clear(),
             c3 = are_all_handlers_finished_handling();
        return c1 && c2 && c3;
	}

	void model::generator_queue_link_th() {
		std::for_each(link_generators_queues.begin(), link_generators_queues.end(), 
			[&](link<generator*, queue*>& link) 
		{
			std::thread([&link, this]()
			{
                for(ull_t i = 1; i <= link.lhs->get_num_requests(); i++)
				{
                    if (!simulate_flag || is_simulating_finished()) //if the user switched simulating off
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
                    if (!simulate_flag || is_simulating_finished()) //if the user switched simulating off
                    {
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

        std::stringstream ss;
        ss << "Simulation started.";
        sLog.writeLine(ss.str());

		generator_queue_link_th();
		handler_queue_link_th();


        std::thread([this]()
        {
            for(; ; )
            {
                std::lock_guard<std::mutex> lk(model_mutex);
                if (!simulate_flag || is_simulating_finished()) //if the user switched simulating off
                {
                    qDebug() << "--------simulation finished" << endl;
                    std::stringstream ss;
                    ss << "Simulation finished.";
                    sLog.writeLine(ss.str());
                    emit simulationFinished();
                    break;
                }
            }
        }).detach();
	}

	void model::simulation_stop() {
		//stops simulating
        simulate_flag = false;
    }


} //end namespace logic