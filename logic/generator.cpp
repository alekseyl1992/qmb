#include "generator.h"
#include "model.h"

#include <numeric>
#include <time.h>

namespace logic
{
    generator::generator(std::string name,
                         int id,
                         QString script,
                         int period,
                         ull_t num_requests,
                         bool is_random,
                         bool is_infinite,
                         bool is_auto_deleting):
        object(ItemType::Generator, name, id),
        generating_period(period),
        number_of_requests_to_generate(num_requests),
        random_generating(is_random),
        infinite_generating(is_infinite),
        auto_deleting(is_auto_deleting),
        count_of_generated_requests(0),
        script(script)
    {
        if (number_of_requests_to_generate == 0 && infinite_generating == true)
            ++number_of_requests_to_generate;
    }

    generator::generator(const generator& gen) :
        object(gen),
        generating_period(gen.generating_period),
        number_of_requests_to_generate(gen.number_of_requests_to_generate),
        random_generating(gen.random_generating),
        infinite_generating(gen.infinite_generating),
        auto_deleting(gen.auto_deleting),
        count_of_generated_requests(gen.count_of_generated_requests)
    { }

    generator::~generator() { }

    void generator::generate_new_request(ull_t req_id)
    {
        std::lock_guard<std::mutex> lk(item_mutex);

        if (req_id == std::numeric_limits<ull_t>::max()) //18,446,744,073,709,551,615 //вотэтоцииииифра
            throw exceptions::TooBigIDException();
        if (random_generating)
        {
            srand(time(NULL));
            generating_period = rand() % 5000;
        }

        engine.globalObject().setProperty("period", generating_period);
        engine.globalObject().setProperty("infinite", infinite_generating);
        engine.globalObject().setProperty("toGenerate", (qsreal)number_of_requests_to_generate);
        engine.globalObject().setProperty("generated", (qsreal)count_of_generated_requests);

        if(!engine.evaluate(script).isError())
        {
            generating_period = engine.globalObject().property("period").toInteger();
            infinite_generating = engine.globalObject().property("infinite").toBool();
            number_of_requests_to_generate = engine.globalObject().property("toGenerate").toNumber();
            count_of_generated_requests = engine.globalObject().property("generated").toInteger();
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(generating_period));
        cur_req = new request(id, req_id);
        moveable_request_flag = true;

        if (parent->is_simulating())
        {
            emit parent->reqGenerated(cur_req->get_id(), get_event_time());
            qDebug() << cur_req->get_id().str_reqID().c_str() << " generated";
        }

        if (infinite_generating)
            ++number_of_requests_to_generate;

        ++count_of_generated_requests;
    }

    void generator::generating()
    {
        for (ull_t ID = 1; ID <= this->get_num_requests() && parent->is_simulating(); ++ID)
        {
            parent->try_pausing(); //если нажата пауза

            if (!this->is_moveable())
                generate_new_request(ID);
            else
            {
                if (auto_deleting)
                {
                    delete cur_req;
                    cur_req = nullptr;
                    moveable_request_flag = true;
                    generate_new_request(ID);
                }
                else
                    --ID;
            }
        }
    }

    request* generator::get_request()
    {
        std::lock_guard<std::mutex> lk(item_mutex);

        moveable_request_flag = false;
        return cur_req;
    }

    void generator::move_request()
    {
        this->generating();
    }

} //end namespace logic
