#include "object.h"
#include "generator.h"
#include "model.h"

namespace logic
{
	//Constructors
    generator::generator(model* parent, int id, int period, ull_t num_requests):
            object(parent, id), new_req(nullptr), generating_period(period), number_of_requests_to_generate(num_requests), is_generated_flag(false) {
		cur_req_id = 0; 
	}

    generator::generator(const generator& gen) : object(gen) {
		new_req = gen.new_req;
		generating_period = gen.generating_period;
		number_of_requests_to_generate = gen.number_of_requests_to_generate;
		is_generated_flag = gen.is_generated_flag;
		cur_req_id = gen.cur_req_id;
	}

	//assignment
	generator& generator::operator=(const generator& gen) {
		if (this == &gen)
			return *this;
        parent = gen.parent;
		new_req = gen.new_req;
		generating_period = gen.generating_period;
		number_of_requests_to_generate = gen.number_of_requests_to_generate;
		is_generated_flag = gen.is_generated_flag;
		cur_req_id = gen.cur_req_id;
        return *this;
    }

    generator::~generator()
    {
    }

	//generating new request
	void generator::generate_new_request() {
        std::lock_guard<std::mutex> lk(gen_mutex);

        std::this_thread::sleep_for(std::chrono::milliseconds(generating_period));
		new_req = new request(id, ++cur_req_id);
		is_generated_flag = true;
		
        emit parent->reqGenerated(new_req->get_id()); //indicates that the new request is generated

        //теперь выглядит громоздко,
        //но было необходимо сделать запись в лог через 1 вызов ф-ции,
        //а не через последовательность вызовов
        /*std::stringstream ss;
        ss << "Request[" << new_req->get_id() << "] generated";
        sLog.writeLine(ss.str());*/
        qDebug() << new_req->get_id().__req_gen_id << "-" << new_req->get_id().__req_id << " generated";
	}

	//gets next request 
	request generator::get_request() {
        std::lock_guard<std::mutex> lk(gen_mutex);

		request res = *new_req;
		delete new_req;
		new_req = nullptr;
		is_generated_flag = false;
		return res;
	}

} //end namespace logic
