#include "model.h"

namespace logic
{
    bool model::are_all_generated() {
        bool all_completed = true;        
        std::for_each(link_generators_queues.begin(), link_generators_queues.end(), [&all_completed](link <generator*, queue*>& link) {
			if (!link.lhs->is_finished())
			{
                all_completed = false;
			}
        });
        return all_completed;
	}

    bool model::are_all_queues_and_handlers_finished_handling()
    {
        bool all_completed = true;
        std::for_each(link_queues_handlers.begin(), link_queues_handlers.end(), [&all_completed](link <queue*, handler*>& link) {
            if (link.lhs->get_size() != 0 || !link.rhs->is_free())
            {
                all_completed = false;
            }
        });
        return all_completed;
    }

    bool model::are_all_terminators_finished_terminating()
    {
        bool all_completed = true;
        std::for_each(link_handlers_terminators.begin(), link_handlers_terminators.end(), [&all_completed](link <handler*, terminator*>& link) {
            if (!link.rhs->is_free())
            {
                all_completed = false;
            }
        });
        return all_completed;
    }

    bool model::is_simulating_finished() {
        bool c1 = are_all_generated(),
             c2 = are_all_queues_and_handlers_finished_handling(),
             c3 = are_all_terminators_finished_terminating();
        return c1 && c2 && c3;
	}

	void model::generator_queue_link_th() {
		std::for_each(link_generators_queues.begin(), link_generators_queues.end(), 
			[&](link<generator*, queue*>& link) 
		{
			std::thread([&link, this]()
			{
                for(; ; )
				{
                    if (!simulate_flag || is_simulating_finished()
                            || link.lhs->is_finished())
                    {
                        //if the user switched simulating off or all requests are generated
						break;
                    }
                    std::unique_lock<std::mutex> lk(model_mutex);
                    if (!link.lhs->is_generated())
                    {
                        link.lhs->generate_new_request();
                        link.rhs->add(link.lhs->get_request());
                    }
				}
			}).detach();
        });
	}

    void model::queue_handler_link_th() {
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
                    std::unique_lock<std::mutex> lk(model_mutex2);
					if (link.lhs->has_request() && link.rhs->is_free())
					{
						link.rhs->handle(link.lhs->get_first());
					}
				}
			}).detach();
    });
    }

    void model::handler_terminator_link_th()
    {
        std::for_each(link_handlers_terminators.begin(), link_handlers_terminators.end(),
                [&](link<handler*, terminator*>& link)
        {
            std::thread([&link, this]()
            {
                for(; ; )
                {
                    if (!simulate_flag || is_simulating_finished()) //if the user switched simulating off
                    {
                        break;
                    }
                    std::unique_lock<std::mutex> lk(model_mutex3);
                    if (link.lhs->is_handled() && link.rhs->is_free())
                    {
                        link.rhs->terminate(link.lhs->get_current_request());
                    }
                }
            }).detach();
        });
    }

	void model::simulation_start() {
		//starts simulating
		simulate_flag = true;
        start_time = clock();

        /*std::stringstream ss;
        ss << "Simulation started.";
        sLog.writeLine(ss.str());*/

		generator_queue_link_th();
        queue_handler_link_th();
        handler_terminator_link_th();


        std::thread([this]()
        {
            for(; ; )
            {
                //std::lock_guard<std::mutex> lk(model_mutex2);
                if (!simulate_flag || is_simulating_finished()) //if the user switched simulating off
                {
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

    void model::add_generator(const generator &&gen)
    {
        generators.emplace_back(gen);
    }

    void model::add_queue(const queue &&q)
    {
        queues.emplace_back(q);
    }

    void model::add_handler(const handler &&h)
    {
        handlers.emplace_back(h);
    }

    void model::add_terminator(const terminator &&t)
    {
        terminators.emplace_back(t);
    }

    void model::add_link_generator_queue(const link<generator *, queue *> &&link)
    {
        link_generators_queues.emplace_back(link);
    }

    void model::add_link_queue_handler(const link<queue *, handler *> &&link)
    {
        link_queues_handlers.emplace_back(link);
    }

    void model::add_link_handler_terminator(const link<handler *, terminator *> &&link)
    {
        link_handlers_terminators.emplace_back(link);
    }

    generator *model::get_generator_by_id(int id)
    {
        std::vector<generator>::iterator iter;
        for(auto it = generators.begin(); it != generators.end(); ++it)
        {
            if (it->get_id() == id)
                iter = it;
        }
        return &(*iter);
    }

    queue *model::get_queue_by_id(int id)
    {
        std::vector<queue>::iterator iter;
        for(auto it = queues.begin(); it != queues.end(); ++it)
        {
            if (it->get_id() == id)
                iter = it;
        }
        return &(*iter);
    }

    handler *model::get_handler_by_id(int id)
    {
        std::vector<handler>::iterator iter;
        for(auto it = handlers.begin(); it != handlers.end(); ++it)
        {
            if (it->get_id() == id)
                iter = it;
        }
        return &(*iter);
    }

    terminator *model::get_terminator_by_id(int id)
    {
        std::vector<terminator>::iterator iter;
        for(auto it = terminators.begin(); it != terminators.end(); ++it)
        {
            if (it->get_id() == id)
                iter = it;
        }
        return &(*iter);
    }


} //end namespace logic
