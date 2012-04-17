#ifndef H_QUEUE
#define H_QUEUE

#include <deque>
#include <mutex>

#include "request.h"
#include "qmbObject.h"

namespace qmodel
{
	class queue : public qmbObject
	{
		std::mutex queue_mutex;
	public:
		queue();
		queue(const queue& q);
		queue& operator=(const queue& q);

		~queue() { }

		
		//adding request to the queue
		void add(request req);
		//get number of elemets in the queue
		int get_size() const { return static_cast<int>(requests_in_queue.size()); }

		//getting next request
		request get_first();
		//getting last request
		request get_last();

		//if the queue has a request
		bool has_request() { 
            //std::lock_guard<std::mutex> lock(queue_mutex);
			return having_request_flag; 
		}
		
	private:
		std::deque<request> requests_in_queue;
		bool having_request_flag;

		static int cur_id;
		
	};
} //end namespace qmodel

#endif // !H_QUEUE
