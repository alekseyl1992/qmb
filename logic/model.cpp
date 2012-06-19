#include <sstream>
#include "model.h"

std::string error_code_str(error_code code)
{
    switch (code)
    {
    case INPUT_IN_GENERATOR:
        return "INPUT_IN_GENERATOR";

    case OUTPUT_IN_TERMINATOR:
        return "OUTPUT_IN_TERMINATOR";
    case NO_GENERATORS:
        return "NO_GENERATORS";
    case NO_TERMINATORS:
        return "NO_TERMINATORS";
    default:
        return "";
    }
}

namespace logic
{
	model::model() :
		simulate_flag(false),
		stop_flag(false),
		pause_flag(false),
		start_time(0),
		num_terminated(0),
		num_generated(0)
	{ }

	model::model(const model& m) :
		generators(m.generators),
		queues(m.queues),
		handlers(m.handlers),
		terminators(m.terminators),

		objects(m.objects),
		errors(m.errors),
		threads(m.threads),

		simulate_flag(m.simulate_flag),
		stop_flag(m.stop_flag),
		pause_flag(m.pause_flag),
		start_time(m.start_time),
		num_terminated(m.num_terminated),
		num_generated(m.num_generated)
	{ }

	model::~model()
	{ }

    bool model::are_all_generated()
	{
        bool all_completed = true;
		num_generated = 0;
        for (auto it = generators.begin(); it != generators.end(); ++it)
        {
			num_generated += it->get_current_num_requests();
            if (!it->is_finished())
            {
                all_completed = false;
                break;
            }
        }

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

        return all_completed;
    }

    bool model::are_all_terminators_finished_terminating()
    {
        bool all_completed = true;
		num_terminated = 0;
        for (auto it = terminators.begin(); it != terminators.end(); ++it)
        {
			num_terminated += it->get_count_of_terminated_requests();
        }
		if (num_terminated != num_generated)  //проверка на то, что все сгенерированные сообщения отработаны
			all_completed = false;

        return all_completed;
    }

    bool model::is_simulating_finished()
	{
        bool c1 = are_all_generated(),
             c2 = are_all_queues_clear(),
             c3 = are_all_handlers_finished_handling(),
             c4 = are_all_terminators_finished_terminating();
		return (c1 && c2 && c3 && c4) || !simulate_flag;
	}

	void model::try_pausing() const
	{
		for (;pause_flag;)
		{
			std::this_thread::sleep_for(std::chrono::nanoseconds(1)); //if paused
		}
	}

    void model::generating_th()
	{
        std::for_each(generators.begin(), generators.end(),
            [&](generator& gen)
		{
			threads.push_back(new std::thread([this, &gen]()
			{
				for (ull_t cur_req_id = 1; cur_req_id <= gen.get_num_requests() && is_simulating(); cur_req_id++)
				{
					try_pausing(); //если нажата пауза

					if (!gen.is_moveable())
						gen.generate_new_request(cur_req_id);
					else
						--cur_req_id;
				}
			}) ); 
		});
    }

	void model::threading()
	{
		std::for_each(objects.begin(), objects.end(),
				[&](object* obj)
		{
			if (obj->get_type() != ItemType::Generator)
			{
				threads.push_back(new std::thread([obj, this]()
				{
					for (;is_simulating(); )
					{
						try_pausing(); //если нажата пауза

						if (obj->has_connection())
                            obj->move_request();
					}
				}) );
			}
		});
	}

	void model::checking_finished_th()
	{
		threads.push_back(new std::thread([this]()
        {
            for (; ; )
            {
				try_pausing(); //если нажата пауза

                if (is_simulating_finished()) //if the user switched simulating off
                {
					simulate_flag = false;
                    emit simulationFinished(static_cast<int>(get_now_time() - start_time));
                    //std::cout << "simulation finished" << std::endl;
                    break;
                }
            }
        }) );
	}

	bool model::is_valid()
	{
		//if no generators found
		if (generators.size() == 0)
			errors.push_back(Pair(nullptr, error_code::NO_GENERATORS));

		//if no terminators found
		if (terminators.size() == 0)
			errors.push_back(Pair(nullptr, error_code::NO_TERMINATORS));

		//search errors in generators and terminators
		std::for_each(objects.begin(), objects.end(), [&](object* obj) {
			if (obj->has_connection())
			{
				if (obj->get_type() == ItemType::Generator)
					errors.push_back(Pair(obj, error_code::INPUT_IN_GENERATOR));

				if (obj->connected_with()->get_type() == ItemType::Terminator)
					errors.push_back(Pair(obj->connected_with(), error_code::OUTPUT_IN_TERMINATOR));
			}
		});

		return !(errors.size());
	}

	std::string model::get_errors() const
	{
		std::stringstream ss;
		ss << "errors occured:\n";
		std::for_each(errors.begin(), errors.end(), [&](std::pair<object const*, error_code> err) {
            ss << "Error #" << err.second << " : " << error_code_str(err.second);
			if (err.first != nullptr)
                ss << " in " << itemTypeTo_stdString(err.first->get_type()).c_str() << " " << err.first->get_id();
			ss << std::endl;
		});
		return ss.str();
	}

	void model::simulation_start()
	{
		stop_flag = false;
		if (pause_flag)
		{
			pause_flag = false;
            emit simulationRestored(static_cast<int>(get_now_time() - start_time));
            //std::cout << "simulation restored" << std::endl;
		}
		else
		{
			simulate_flag = true;
            start_time = get_now_time();
            emit simulationStarted(static_cast<int>(start_time));
            //std::cout << "simulation started" << std::endl;
			
			generating_th();
			threading();
			checking_finished_th();
		}
		if (!is_simulating())
		{
			std::for_each(threads.begin(), threads.end(), [](std::thread* th)
			{
				th->join();
			});
		}
	}

    void model::simulation_stop()
	{
        stop_flag = true;
		if (simulate_flag) //для корректного вывода
		{
            emit simulationStopped(static_cast<int>(get_now_time() - start_time));
            //std::cout << "simulation stopped" << std::endl;
		}
	}
	
	void model::simulation_pause()
	{
		pause_flag = true;
        emit simulationPaused(static_cast<int>(get_now_time() - start_time));
        //std::cout << "simulation paused" << std::endl;
	}

    void model::add_generator(generator &&gen)
    {
		gen.set_parrent(this);
        generators.emplace_back(gen);
		objects.push_back(&generators.back());
    }

    void model::add_queue(queue &&q)
    {
		q.set_parrent(this);
        queues.emplace_back(q);
		objects.push_back(&queues.back());
    }

    void model::add_handler(handler &&h)
    {
		h.set_parrent(this);
        handlers.emplace_back(h);
		objects.push_back(&handlers.back());
    }

    void model::add_terminator(terminator &&t)
    {
		t.set_parrent(this);
        terminators.emplace_back(t);
		objects.push_back(&terminators.back());
    }

	void model::connect(object* lhs, object* rhs)
	{
        rhs->connect_with(lhs);
    }

    object* model::find_object(ull_t global_id)
    {
        std::list<object*>::iterator iter;
        for(auto it = objects.begin(); it != objects.end(); ++it)
        {
            if (it->get_global_id() == global_id)
            {
                iter = it;
                break;
            }
        }
        return &(*iter);
    }

    generator* model::find_generator(ull_t id)
    {
		std::list<generator>::iterator iter;
		for(auto it = generators.begin(); it != generators.end(); ++it)
        {
            if (it->get_id() == id)
			{
                iter = it;
				break;
			}
        }
        return &(*iter);
    }

    queue* model::find_queue(ull_t id)
    {
        std::list<queue>::iterator iter;
        for(auto it = queues.begin(); it != queues.end(); ++it)
        {
            if (it->get_id() == id)
            {
                iter = it;
				break;
			}
        }
        return &(*iter);
    }

    handler* model::find_handler(ull_t id)
    {
        std::list<handler>::iterator iter;
        for(auto it = handlers.begin(); it != handlers.end(); ++it)
        {
            if (it->get_id() == id)
            {
                iter = it;
				break;
			}
        }
        return &(*iter);
    }

    terminator* model::find_terminator(ull_t id)
    {
        std::list<terminator>::iterator iter;
        for(auto it = terminators.begin(); it != terminators.end(); ++it)
        {
            if (it->get_id() == id)
            {
                iter = it;
				break;
			}
        }
        return &(*iter);
    }

	/*
    std::vector< std::pair<ItemType, ItemType> > model::supportedLinks()
    {
        typedef std::pair<ItemType, ItemType> pair;
        std::vector<pair> links;

        links.emplace_back(pair(ItemType::Generator, ItemType::Queue));
        links.emplace_back(pair(ItemType::Queue, ItemType::Handler));
        links.emplace_back(pair(ItemType::Handler, ItemType::Terminator));

        return links;
    }*/


} //end namespace logic
