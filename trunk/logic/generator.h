#ifndef H_GENERATOR
#define H_GENERATOR

#include <thread>
#include "object.h"

#include "request.h"
#include "exceptions.h"

namespace logic
{

    class object;
    class model;

    class generator : public object
	{
	public:
        generator(model* parent, int period = 0, ull_t num_requests = 0);
		generator(const generator& gen);
		generator& operator=(const generator& gen);

        ~generator();


		//generating new request
		void generate_new_request();
		//gets next request 
		request get_request();

		//gets generating period
		int get_generating_period() const { return generating_period; }
		//sets generating period
		void set_generating_period(int period) { generating_period = period; }

		//gets generated flag
		bool is_generated() const { return is_generated_flag; }

		//gets number of required requests
        ull_t get_num_requests() const { return number_of_requests_to_generate; }
		//sets number of required requests
        void set_num_requests(ull_t num) { number_of_requests_to_generate = num; }

		bool is_finished() const {
			return cur_req_id >= number_of_requests_to_generate;
		}

		virtual void clean() { }

        static int cur_id; //<- генератор

	private:
        //Fields
		request* new_req;
		int generating_period;
        ull_t number_of_requests_to_generate;

		bool is_generated_flag;

        ull_t cur_req_id; //<- запрос
	};


} //end namespace logic

#endif // !H_GENERATOR
