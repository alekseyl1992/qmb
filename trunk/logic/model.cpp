#include <sstream>
#include <string>
#include "model.h"

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

    void model::new_thread(object* obj)
    {
        threads.push_back(new std::thread([obj, this]()
        {
            while (is_simulating())
            {
                try_pausing(); //если нажата пауза
                obj->move_request();
            }
        }) );
    }

    void model::threading()
    {
        std::for_each(objects.begin(), objects.end(),
                [&](object* obj)
        {
              if (obj->has_input())
              {
                  if (obj->get_type() != ItemType::Collector)
                  {
                      //if (obj->input_connection()->get_type() != ItemType::Separator)
                      //{
                          new_thread(obj);
                      //}
                  }
                  else  //if obj is Collector
                      new_thread(obj);
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

                if (is_simulating_finished())
                {
                    if (stop_flag)
                        break;
                    simulate_flag = false;

                    emit simulationFinished(static_cast<int>(get_now_time() - start_time));
                    qDebug() << "simulation finished" << endl;

                    break;
                }
        }
        }) );
    }

    bool model::ExitPointSearch(object *obj)
    {
        if (obj->has_output())
        {
            for(object* output_obj : obj->output_connection())
            {
                if (output_obj->get_type() == ItemType::Terminator)
                    return true;
                else
                    return ExitPointSearch(output_obj);
            }
        }
        return false;
    }

    bool model::HasExitPoint(object *obj)
    {
        return ExitPointSearch(obj);
    }

    std::string model::intToString(int val)
    {
        int digitsCount = 0;
        int tmp = val;
        while(tmp > 0)
        {
            tmp /= 10;
            ++digitsCount;
        }

        char* txt_num = new char[digitsCount + 1];
        itoa(val, txt_num, 10);

        std::string res(txt_num);

        delete txt_num;
        txt_num = nullptr;

        return res;
    }

    bool model::is_valid()
    {
        /*old checking
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
*/

        if (generators.size() == 0)
            throw exceptions::NoGeneratorsException();

        if(terminators.size() == 0)
            throw exceptions::NoTerminatorsException();

        std::vector<generator*> bad_generators;
        for(generator& gen : generators)
        {
            if(!HasExitPoint(&gen))
            {
                bad_generators.push_back(&gen);
            }
        }

        if (bad_generators.size() != 0)
        {
            std::string _message = "Не для всех генераторов существует выходная точка:\n[id]";
            for (auto it = bad_generators.begin(); it != bad_generators.end() - 1; ++it)
            {
                _message += intToString((*it)->get_id()) + ", ";
            }
            _message += intToString((*(bad_generators.end() - 1))->get_id());

            throw exceptions::NoExitPointException(_message);
        }

        return true;
    }

    void model::simulation_start()
    {
        try
        {
            is_valid();
        }
        catch(exceptions::LogicException& ex)
        {
            throw ex;
        }
        simulate_flag = true;
        stop_flag = false;
        pause_flag = false;

        start_time = get_now_time();
        emit simulationStarted(0);
        qDebug() << "simulation started";

        generating_th();
        threading();
        checking_finished_th();

        if (is_simulating_finished())
        {
            std::for_each(threads.begin(), threads.end(), [](std::thread* th)
            {
                th->join();
            });
        }
    }

    void model::simulation_stop()
    {
        simulate_flag = false;
        stop_flag = true;
        pause_flag = false;

        emit simulationStopped(static_cast<int>(get_now_time() - start_time));
        qDebug() << "simulation stopped";
    }

    void model::simulation_pause()
    {
        pause_flag = true;

        emit simulationPaused(static_cast<int>(get_now_time() - start_time));
        qDebug() << "simulation paused";
    }

    void model::simulation_restore()
    {
        if (pause_flag)
        {
            pause_flag = false;

            emit simulationRestored(static_cast<int>(get_now_time() - start_time));
            qDebug() << "simulation restored";
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

    void model::add_collector(collector &&col)
    {
        col.set_parrent(this);
        collectors.emplace_back(col);
        objects.push_back(&collectors.back());
    }

    void model::add_separator(separator &&sep)
    {
        sep.set_parrent(this);
        separators.emplace_back(sep);
        objects.push_back(&separators.back());
    }

    void model::connect(object* lhs, object* rhs)
    {
        lhs->set_output(rhs);
        rhs->set_input(lhs);
    }

    object* model::find_object(int global_id)
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

    generator* model::find_generator(int id)
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

    queue* model::find_queue(int id)
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

    handler* model::find_handler(int id)
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

    terminator* model::find_terminator(int id)
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

    collector* model::find_collector(int id)
    {
        std::list<collector>::iterator iter;
        for(auto it = collectors.begin(); it != collectors.end(); ++it)
        {
            if (it->get_id() == id)
            {
                iter = it;
                break;
            }
        }
        return &(*iter);
    }

    separator* model::find_separator(int id)
    {
        std::list<separator>::iterator iter;
        for(auto it = separators.begin(); it != separators.end(); ++it)
        {
            if (it->get_id() == id)
            {
                iter = it;
                break;
            }
        }
        return &(*iter);
    }

    void model::add_attribute(attribute &attr)
    {
        attributes.add(attr);
    }

    void model::remove_attribute(int id)
    {
        attributes.erase(id);
    }

    attribute& model::get_attribute(int id)
    {
        return attributes.get(id);
    }




} //end namespace logic
