#ifndef H_GENERATOR
#define H_GENERATOR

#include <thread>

#include "request.h"
#include "exceptions.h"
#include "qmbObject.h"

namespace qmodel
{
	class generator : public qmbObject
	{
	public:
        generator(int period = 0);
        generator(const generator& gen);
		generator& operator=(const generator& gen);

		~generator() { delete new_req; }


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

	private:
		//Fields
		request* new_req;
		int generating_period;
		bool is_generated_flag;

		static int cur_id; //<- генератор
		int cur_req_id; //<- запрос
	};

} //end namespace qmodel

#endif // !H_GENERATOR
