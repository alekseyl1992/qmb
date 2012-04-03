#ifndef H_REQUEST_GENERATOR
#define H_REQUEST_GENERATOR

#include <thread>
#include "request.h"
#include "exceptions.h"
#include <QDebug>

namespace qmodel
{

	template<typename Type = int>
	class request_generator
	{
	public:
		
		request_generator();
		request_generator(std::chrono::milliseconds period);
		~request_generator() { delete new_req; }

		//Function generates new request
		void generate_request(Type content);
		//generating new random request
		void generate_new_request();
		//gets next request 
		request<Type> get_request();
	
		bool is_generated() const { return is_generated_flag; }

	private:
		//Fields
		request<Type>* new_req;
		std::chrono::milliseconds generating_period;
		static Type id;
		bool is_generated_flag;
	};


	/**********************Implementation**********************/

	template<typename Type> Type request_generator<Type>::id = 0;

	//Constructors
	template<typename Type> inline
		request_generator<Type>::request_generator(): 
			new_req(nullptr), is_generated_flag(false), generating_period(std::chrono::milliseconds(1000)) 
	{ }

	template<typename Type> inline
		request_generator<Type>::request_generator(std::chrono::milliseconds period): 
			new_req(nullptr), is_generated_flag(false), generating_period(period) 
	{ }

	//generating new request
	template<typename Type>
		void request_generator<Type>::generate_request(Type id) {
			new_req = new request<Type>(id);
	}

	//generating new random request
	template<typename Type>
		void request_generator<Type>::generate_new_request() {
			std::this_thread::sleep_for(generating_period);
			generate_request(++id);
			is_generated_flag = true;
            qDebug() << "Request[" << id << "] generated" << endl;
	}

	//gets next request 
	template<typename Type>
		request<Type> request_generator<Type>::get_request() {
			if (new_req == nullptr)
				throw qmodel::exceptions::no_requests();
			request<Type> res = *new_req;
			new_req = nullptr;
			is_generated_flag = false;
			return res;
	}

} //end namespace qmodel

#endif // !H_REQUEST_GENERATOR
