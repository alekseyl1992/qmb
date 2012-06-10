#include "generator.h"
#include "model.h"

namespace logic
{
    generator::generator(ull_t id, int period, ull_t num_requests, bool is_random, bool is_infinite):
		object(ItemType::Generator, id), 
		generating_period(period), 
		number_of_requests_to_generate(num_requests), 
		random_generating(is_random),
		infinite_generating(is_infinite), 
		count_of_generated_requests(0)
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
		count_of_generated_requests(gen.count_of_generated_requests)
	{ }

    generator::~generator() { }

	void generator::generate_new_request(ull_t req_id) 
	{
        std::lock_guard<std::mutex> lk(item_mutex);

        std::this_thread::sleep_for(std::chrono::milliseconds(generating_period));
        
		cur_req = new request(id, req_id);
		moveable_request_flag = true;

		if (parent->is_simulating())
		{
            emit parent->reqGenerated(cur_req->get_id(), static_cast<int>(get_now_time() - parent->get_start_time())); //indicates that the new request is generated
            //std::cout << cur_req->get_id().str_reqID() << " generated" << endl;
		}

		if (infinite_generating == true)
			++number_of_requests_to_generate;

		++count_of_generated_requests;
	}

	request* generator::get_request() 
	{
        std::lock_guard<std::mutex> lk(item_mutex);

		moveable_request_flag = false;
		return cur_req;
	}

} //end namespace logic
