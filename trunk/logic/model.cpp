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
        start_time(0)
    { }

    model::~model()
    {
        std::for_each(objects.begin(), objects.end(), [](object* obj)
        {
            delete obj;
            obj = nullptr;
        });
    }


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
            ull_t num_generated = request::get_generated_count();
            ull_t num_terminated = request::get_deleted_count();

            if (num_generated != num_terminated)  //проверка на то, что все сгенерированные сообщения отработаны
                all_completed = false;
        }

        return all_completed || !simulate_flag;
    }

    bool model::is_simulating() const
    {
        return simulate_flag && !stop_flag;
    }

    void model::try_pausing() const
    {
        for (;pause_flag;)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(1)); //if paused
        }
    }

    bool model::is_paused() const
    {
        return pause_flag;
    }

    ull_t model::get_start_time() const
    {
        return start_time;
    }

    void model::changeCondition(model::simulationCondition cond)
    {
        switch(cond)
        {
        case simulationCondition::Start:
            simulate_flag = true;
            stop_flag = false;
            pause_flag = false;
            break;
        case simulationCondition::Stop:
            simulate_flag = false;
            stop_flag = true;
            pause_flag = false;
            break;
        case simulationCondition::Pause:
            pause_flag = true;
            break;
        case simulationCondition::Restore:
            pause_flag = false;
            break;
        default:
            break;
        }
    }

    void model::generating_th()
    {
        for (object* gen : generators)
        {
            threads.push_back(new std::thread([this, gen]()
            {
                gen->move_request(); //invokes generating()
            }) );
        }
    }

    bool model::thread_necessary(object *obj)
    {
        bool res = false;
        if (obj->has_input())
        {
            if (obj->get_type() != ItemType::Collector && obj->get_type() != ItemType::Separator)
            {
                if (obj->input()->get_type() != ItemType::Separator)
                {
                    res = true;
                }
            }
            else  //if obj is Collector or Separator
                res = true;
        }
        return res;
    }

    void model::new_thread(object* obj)
    {
        threads.push_back(new std::thread([this, obj]()
        {
            try
            {
                while (is_simulating())
                {
                    try_pausing(); //если нажата пауза
                    obj->move_request();
                }
            }
            catch(exceptions::LogicException)
            {
                stop_reason = std::current_exception();
                simulation_stop();
            }
        }) );
    }

    void model::threading()
    {
        for(object* obj : objects)
        {
            if(!simulate_flag)
                return;
            else if (thread_necessary(obj))
                new_thread(obj);
        }
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

    bool model::is_valid()
    {
        if (generators.size() == 0)
            throw exceptions::NoGeneratorsException();

        if (terminators.size() == 0)
            throw exceptions::NoTerminatorsException();

        //проверка объектов на наличие нескольких входов или выходов (не относится к Коллекторам и Сепараторам)
        std::vector<object*> bad_inputs_objects,
                             bad_outputs_objects;
        for(object* obj : objects)
        {
            if(obj->inputs_count() > 1 && obj->get_type() != ItemType::Collector)
            {
                bad_inputs_objects.push_back(obj);
            }
            if(obj->outputs_count() > 1 && obj->get_type() != ItemType::Separator)
            {
                bad_outputs_objects.push_back(obj);
            }
        }
        if (bad_inputs_objects.size() != 0 || bad_outputs_objects.size() != 0)
            throw exceptions::ManyInputsOutputsException(bad_inputs_objects, bad_outputs_objects);

        //Проверка на существование "точки выхода"
        std::vector<object*> bad_generators;
        for(object* gen : generators)
        {
            if(!HasExitPoint(gen))
                bad_generators.push_back(gen);
        }
        if (bad_generators.size() != 0)
            throw exceptions::NoExitPointException(bad_generators);

        return true;
    }

    void model::simulation_start()
    {
        changeCondition(simulationCondition::Start);

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
        changeCondition(simulationCondition::Stop);

        emit simulationStopped(static_cast<int>(get_now_time() - start_time));
        qDebug() << "simulation stopped";
    }

    void model::simulation_pause()
    {
        changeCondition(simulationCondition::Pause);

        emit simulationPaused(static_cast<int>(get_now_time() - start_time));
        qDebug() << "simulation paused";
    }

    void model::simulation_restore()
    {
        if (pause_flag)
        {
            changeCondition(simulationCondition::Restore);

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
        auto iter = std::find_if(objects.begin(), objects.end(), [=](object* obj) {
            return obj->get_global_id() == global_id;
        });
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
