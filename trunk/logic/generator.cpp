#include "generator.h"


namespace logic
{
	int generator::cur_id = 0;

	//Constructors
    generator::generator(int period, ull_t num_requests):
            object(++cur_id), new_req(NULL), generating_period(period), number_of_requests_to_generate(num_requests), is_generated_flag(false) {
		cur_req_id = 0; 
	}

	generator::generator(const generator& gen)  {
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
		new_req = gen.new_req;
		generating_period = gen.generating_period;
		number_of_requests_to_generate = gen.number_of_requests_to_generate;
		is_generated_flag = gen.is_generated_flag;
		cur_req_id = gen.cur_req_id;
		return *this;
	}

	//generating new request
	void generator::generate_new_request() {
		std::this_thread::sleep_for(std::chrono::milliseconds(generating_period));

		new_req = new request(id, ++cur_req_id);
		is_generated_flag = true;
		
        //теперь выглядит громоздко,
        //но было необходимо сделать запись в лог через 1 вызов ф-ции,
        //а не через последовательность вызовов
        std::stringstream ss;
        ss << "Request[" << new_req->get_id() << "] generated";
        sLog.writeLine(ss.str());
	}

	//gets next request 
	request generator::get_request() {
		request res = *new_req;
		delete new_req;
		new_req = nullptr;
		is_generated_flag = false;
		return res;
	}

} //end namespace logic
