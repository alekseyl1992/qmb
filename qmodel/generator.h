#ifndef H_GENERATOR
#define H_GENERATOR

#include <thread>


#include "request.h"
#include "exceptions.h"
#include "object.h"

namespace qmodel
{
    class generator : public object
	{
	public:
		generator(int period = 0, unsigned long long num_requests = 0);
		generator(const generator& gen);
		generator& operator=(const generator& gen);

		~generator() { }


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
		unsigned long long get_num_requests() const { return number_of_requests_to_generate; }
		//sets number of required requests
		void set_num_requests(unsigned long long num) { number_of_requests_to_generate = num; }

		bool is_finished() const {
			return cur_req_id >= number_of_requests_to_generate;
		}

		virtual void clean() { }

	private:
		//Fields
		request* new_req;
		int generating_period;
		unsigned long long number_of_requests_to_generate;

		bool is_generated_flag;

		static int cur_id; //<- генератор
		int cur_req_id; //<- запрос
	};

} //end namespace qmodel

#endif // !H_GENERATOR
