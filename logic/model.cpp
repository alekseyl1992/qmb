#include "model.h"

namespace logic
{
    bool model::are_all_generated() {
        bool all_completed = true;
        for (auto it = generators.begin(); it != generators.end(); ++it)
        {
            if (!it->is_finished())
            {
                all_completed = false;
                break;
            }
        }
        /*std::for_each(link_generators_queues.begin(), link_generators_queues.end(), [&all_completed](link <generator*, queue*>& link) {
			if (!link.lhs->is_finished())
			{
                all_completed = false;
			}
        });*/
        return all_completed;
	}

    bool model::are_all_queues_clear()
    {
        bool all_completed = true;
        for (auto it = queues.begin(); it != queues.end(); ++it)
        {
            if (it->get_size() != 0)
            {
                all_completed = false;
                break;
            }
        }
        /*std::for_each(link_queues_handlers.begin(), link_queues_handlers.end(), [&all_completed](link <queue*, handler*>& link) {
            if (link.lhs->get_size() != 0)
            {
                all_completed = false;
            }
        });*/
        return all_completed;
    }

    bool model::are_all_handlers_finished_handling()
    {
        bool all_completed = true;
        for (auto it = handlers.begin(); it != handlers.end(); ++it)
        {
            if (!it->is_free())
            {
                all_completed = false;
                break;
            }
        }
        /*std::for_each(link_queues_handlers.begin(), link_queues_handlers.end(), [&all_completed](link <queue*, handler*>& link) {
            if (!link.rhs->is_free())
            {
                all_completed = false;
            }
        });*/
        return all_completed;
    }

    bool model::are_all_terminators_finished_terminating()
    {
        bool all_completed = true;
        for (auto it = terminators.begin(); it != terminators.end(); ++it)
        {
            if (!it->is_free())
            {
                all_completed = false;
                break;
            }
        }
        /*std::for_each(link_handlers_terminators.begin(), link_handlers_terminators.end(), [&all_completed](link <handler*, terminator*>& link) {
            if (!link.rhs->is_free())
            {
                all_completed = false;
            }
        });*/
        return all_completed;
    }

    bool model::is_simulating_finished() {
        bool c1 = are_all_generated(),
             c2 = are_all_queues_clear(),
             c3 = are_all_handlers_finished_handling(),
             c4 = are_all_terminators_finished_terminating();
        return c1 && c2 && c3 && c4;
	}

    void model::generating_th() {
        std::for_each(generators.begin(), generators.end(),
            [&](generator& gen)
		{
            std::thread([&gen, this]()
			{
                for(;simulate_flag /*&& !is_simulating_finished()*/ && !gen.is_finished(); )
				{
                    //std::unique_lock<std::mutex> lk(model_mutex);
                    if (!gen.is_generated())
                    {
                        gen.generate_new_request();
                    }
				}
			}).detach();
        });
    }

    void model::queueing_th()
    {
        std::for_each(link_generators_queues.begin(), link_generators_queues.end(),
            [&](link<generator*, queue*>& link)
        {
            std::thread([&link, this]()
            {
                for(;simulate_flag && !is_simulating_finished(); )
                {
                    //std::unique_lock<std::mutex> lk(model_mutex);
                    if (link.lhs->is_generated())
                    {
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
                for(;simulate_flag && !is_simulating_finished(); )
				{
                    //std::unique_lock<std::mutex> lk(model_mutex);
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
                for(;simulate_flag && !is_simulating_finished(); )
                {
                    //std::unique_lock<std::mutex> lk(model_mutex);
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

        start_time = get_now_time();

        generating_th();
        queueing_th();
        queue_handler_link_th();
        handler_terminator_link_th();


        std::thread([this]()
        {
            for(; ; )
            {
                if (!simulate_flag || is_simulating_finished()) //if the user switched simulating off
                {
                    simulate_flag = false;
                    emit simulationFinished(static_cast<int>(get_now_time() - start_time));
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

    std::vector< std::pair<ItemType, ItemType> > model::supportedLinks()
    {
        typedef std::pair<ItemType, ItemType> pair;
        std::vector<pair> links;

        links.emplace_back(pair(ItemType::Generator, ItemType::Queue));
        links.emplace_back(pair(ItemType::Queue, ItemType::Handler));
        links.emplace_back(pair(ItemType::Handler, ItemType::Terminator));

        return links;
    }


} //end namespace logic
