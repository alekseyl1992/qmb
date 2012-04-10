#ifndef H_HANDLER
#define H_HANDLER

#include "request.h"
#include <ostream>

namespace qmodel
{
	template<typename Type = int>
	class handler
	{
	public:
		handler();
		handler(std::chrono::milliseconds _handlePeriod);
		~handler() { delete cur_req; }

		//Check the busy flag of the handler
		bool is_free() const { return freedom_flag; }
		//Send request to handler
		void handle(request<Type> req);

	private:
		//finish the request
		void finish_handling();

		//fields
		request<Type>* cur_req;
		std::chrono::milliseconds handling_period;
		bool freedom_flag;
	};

	/**********************Implementation**********************/

	//Constructors
	template<typename Type> inline
		handler<Type>::handler(): 
            cur_req(nullptr), handling_period(std::chrono::milliseconds(5000)), freedom_flag(true)
		{ }


	template<typename Type> inline
		handler<Type>::handler(std::chrono::milliseconds _handlePeriod): 
            cur_req(nullptr), handling_period(_handlePeriod), freedom_flag(true)
		{ }

	//Send request to handler
	template<typename Type> inline
		void handler<Type>::handle(request<Type> req) {
            sLog << "---Request[" << req.get_id() << "] was put to the handler" << endl;
			freedom_flag = false;
			cur_req = new request<Type>(req);
			std::this_thread::sleep_for(std::chrono::milliseconds(handling_period));
			finish_handling();
	}

	//finish the request
	template<typename Type> inline
		void handler<Type>::finish_handling() {
            sLog << "----Request[" << cur_req->get_id() << "] finished handling" << endl;
			freedom_flag = true;
	}

} //end namespace qmodel

#endif // !H_HANDLER
