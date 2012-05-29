#ifndef H_GENERATOR
#define H_GENERATOR



#include "object.h"
#include "request.h"
#include "exceptions.h"

namespace logic
{
    class object;
    class model;

    //! Класс является элементом модели (класс logic::model).
    /*!
     * Представляет собой объект, испольуемый для генерации сообщений (запросов) в
     * модели logic::model. Является абсолютно независимым элементом СМО, не имеет входа, может иметь несколько
     * выходов, распределение запросов по связям распределяется случайным образом.
     */
    class generator : public object
    {
        std::mutex gen_mutex;
	public:
        generator(model* parent, int id=0, int period = 0, ull_t num_requests = 0, bool is_infinite = false);
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
        //gets number of requests
        ull_t get_current_num_requests() const { return cur_req_id; }
		//sets number of required requests
        void set_num_requests(ull_t num) { number_of_requests_to_generate = num; }
        //if the generator has to generate requests infinetly
        bool is_infinite() const { return infinite_generating; }

		bool is_finished() const {
            return cur_req_id >= number_of_requests_to_generate && new_req == nullptr;
		}

		virtual void clean() { }

	private:
        //Fields
		request* new_req;
		int generating_period;
        bool infinite_generating;

        ull_t number_of_requests_to_generate;
		bool is_generated_flag;
        ull_t cur_req_id; //<- запрос
	};


} //end namespace logic

#endif // !H_GENERATOR
