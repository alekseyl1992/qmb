#include <sstream>
#include <string>
#include <algorithm>
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
                if ((*it)->has_output())
                    num_generated += (dynamic_cast<generator*>(*it))->get_current_num_requests();

            for (auto it = terminators.begin(); it != terminators.end(); ++it)
                num_terminated += (dynamic_cast<terminator*>(*it))->get_count_of_terminated_requests();

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
            [&](object* gen)
        {
            threads.push_back(new std::thread([this, gen]()
            {
                for (ull_t cur_req_id = 1; cur_req_id <= (dynamic_cast<generator*>(gen))->get_num_requests() && is_simulating(); cur_req_id++)
                {
                    try_pausing(); //если нажата пауза

                    if (!gen->is_moveable())
                        (dynamic_cast<generator*>(gen))->generate_new_request(cur_req_id);
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
                  if (obj->get_type() != ItemType::Collector && obj->get_type() != ItemType::Separator)
                  {
                      if (obj->input()->get_type() != ItemType::Separator)
                      {
                          new_thread(obj);
                      }
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
        bool result = false;
        if (obj->has_output())
        {
            std::vector<bool> outputs_res;
            for(object* output_obj : obj->output_connection())
            {
                if (output_obj->get_type() == ItemType::Terminator)
                    outputs_res.push_back(true);
                else
                    outputs_res.push_back(ExitPointSearch(output_obj));
            }
            int falsesCount = (int)std::count(outputs_res.begin(), outputs_res.end(), false);
            result = falsesCount == 0 ? true : false;
        }
        return result;
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
        if (generators.size() == 0)
            throw exceptions::NoGeneratorsException();

        if (terminators.size() == 0)
            throw exceptions::NoTerminatorsException();

        std::vector<object*> bad_generators;

        for(object* gen : generators)
        {
            if(!HasExitPoint(gen))
            {
                bad_generators.push_back(gen);
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

    void model::add_object(object *obj)
    {
        obj->set_parent(this);
        objects.push_back(obj);
        switch(obj->get_type())
        {
        case ItemType::Generator:
            generators.push_back(obj);
            break;
        case ItemType::Queue:
            queues.push_back(obj);
            break;
        case ItemType::Handler:
            handlers.push_back(obj);
            break;
        case ItemType::Terminator:
            terminators.push_back(obj);
            break;
        case ItemType::Collector:
            collectors.push_back(obj);
            break;
        case ItemType::Separator:
            separators.push_back(obj);
            break;
        default:
            break;
        }
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

    void model::connect(object* lhs, object* rhs)
    {
        lhs->set_output(rhs);
        rhs->set_input(lhs);
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
