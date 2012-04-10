#ifndef H_QUEUE
#define H_QUEUE

#include <deque>
#include "request.h"

namespace qmodel
{
	
	template<typename Type = int>
	class queue
	{
	public:
		queue(): having_request_flag(false) { }
		~queue() { }

		//adding request to the queue
		void add(request<Type> req);
		//get number of elemets in the queue
		int get_size() const { return static_cast<int>(requests_in_queue.size()); }
		//getting next request
		request<Type> get_first();
		//getting last request
		request<Type> get_last();
		//if the queue has a request
		bool has_request() const { return having_request_flag; }
		
	private:
		std::deque< request<Type> > requests_in_queue;
		bool having_request_flag;

		
	};

	/**********************Implementation**********************/

	//adding request to the queue
	template<typename Type>
		void queue<Type>::add(request<Type> req) {
            sLog << "-Request[" << req.get_id() << "] was put to the queue" << endl;
			requests_in_queue.push_back(req);
			having_request_flag = true;
	}

	//getting next request
	template<typename Type>
		request<Type> queue<Type>::get_first() {
			request<Type> res = *(requests_in_queue.begin());
			requests_in_queue.pop_front();
			if(requests_in_queue.size() == 0)
				having_request_flag = false;
			return res;
	}

	//getting last request
	template<typename Type>
		request<Type> queue<Type>::get_last() {
			request<Type> res = *(requests_in_queue.end() - 1);
			requests_in_queue.pop_back();
			if(requests_in_queue.size() == 0)
				having_request_flag = false;
			return res;
	}

} //end namespace qmodel

#endif // !H_QUEUE
