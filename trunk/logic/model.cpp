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


    bool model::is_simulating_finished()
	{
        bool all_completed = true;
        for(auto it = objects.begin(); it != objects.end(); ++it)
        {
            if (!(*it)->is_completed())
            {
                all_completed = false;
                break;
            }
        }
        if (all_completed)
        {
            num_generated = 0;
            num_terminated = 0;
            for (auto it = generators.begin(); it != generators.end(); ++it)
                if (it->has_output())
                    num_generated += it->get_current_num_requests();

            for (auto it = terminators.begin(); it != terminators.end(); ++it)
                num_terminated += it->get_count_of_terminated_requests();

            if (num_terminated != num_generated)  //проверка на то, что все сгенерированные сообщения отработаны
                all_completed = false;
        }

        return all_completed || !simulate_flag;
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
              if (obj->has_input())
              {
                  threads.push_back(new std::thread([obj, this]()
                  {
                      for (;is_simulating(); )
                      {

                          try_pausing(); //если нажата пауза
                          obj->move_request();
                      }
                  }) );
              }
		});
	}

    /*void model::checking_finished_th()
    {
        static int count = 0;
        count++;
        if (count == 1)
        {
            simulate_flag = false;
            emit simulationFinished(static_cast<int>(get_now_time() - start_time));
            //std::cout << "simulation finished" << std::endl;
        }
    }*/

    void model::checking_finished_th()
	{
		threads.push_back(new std::thread([this]()
        {
            for (; ; )
            {
				try_pausing(); //если нажата пауза

                if (is_simulating_finished())
                {
                    if (stop_flag)
                        break;
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
            if (obj->has_input())
			{
				if (obj->get_type() == ItemType::Generator)
					errors.push_back(Pair(obj, error_code::INPUT_IN_GENERATOR));
            }
            if (obj->has_output())
            {
                if (obj->get_type() == ItemType::Terminator)
                    errors.push_back(Pair(obj, error_code::OUTPUT_IN_TERMINATOR));
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
        simulate_flag = true;
        stop_flag = false;
        pause_flag = false;

        start_time = get_now_time();
        emit simulationStarted(0);
        //std::cout << "simulation started" << std::endl;

        generating_th();
        threading();
        checking_finished_th();

        std::for_each(threads.begin(), threads.end(), [](std::thread* th)
        {
            th->detach();
        });
	}

    void model::simulation_stop()
	{
        simulate_flag = false;
        stop_flag = true;
        pause_flag = false;

        emit simulationStopped(static_cast<int>(get_now_time() - start_time));
        //std::cout << "simulation stopped" << std::endl;
	}
	
	void model::simulation_pause()
	{
		pause_flag = true;

        emit simulationPaused(static_cast<int>(get_now_time() - start_time));
        //std::cout << "simulation paused" << std::endl;
    }

    void model::simulation_restore()
    {
        if (pause_flag)
        {
            pause_flag = false;

            emit simulationRestored(static_cast<int>(get_now_time() - start_time));
            //std::cout << "simulation restored" << std::endl;
        }
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
        lhs->set_output(rhs);
        rhs->set_input(lhs);
    }

    object* model::find_object(ull_t global_id)
    {
        std::vector<object*>::iterator iter;
        for(auto it = objects.begin(); it != objects.end(); ++it)
        {
            if ((*it)->get_global_id() == global_id)
            {
                iter = it;
                break;
            }
        }
        return *iter;
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


} //end namespace logic
