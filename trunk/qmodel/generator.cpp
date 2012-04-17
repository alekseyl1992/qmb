#include "generator.h"

namespace qmodel
{
	/**********************Implementation**********************/

	int generator::cur_id = 0;

	//Constructors
    generator::generator(int period):
			qmbObject(++cur_id), new_req(nullptr), generating_period(period) , is_generated_flag(false) { 
		cur_req_id = 0; 
	}

    generator::generator(const generator& gen)  {
		new_req = gen.new_req;
		generating_period = gen.generating_period;
		is_generated_flag = gen.is_generated_flag;
		cur_req_id = gen.cur_req_id;
	}

	//assignment
    generator& generator::operator=(const generator& gen) {
		if (this == &gen)
			return *this;
		new_req = gen.new_req;
		generating_period = gen.generating_period;
		is_generated_flag = gen.is_generated_flag;
		cur_req_id = gen.cur_req_id;
		return *this;
	}

	//generating new request
	void generator::generate_new_request() {
		std::this_thread::sleep_for(std::chrono::milliseconds(generating_period));

		new_req = new request(id, ++cur_req_id);
		is_generated_flag = true;
		
        sLog << "Request[" << new_req->get_id() << "] generated" << endl;
	}

	//gets next request 
    request generator::get_request() {
        if (new_req == nullptr)
			throw qmodel::exceptions::no_requests();
		request res = *new_req;
		new_req = nullptr;
		is_generated_flag = false;
		return res;
	}

} //end namespace qmodel
