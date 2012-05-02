#ifndef H_QUEUE
#define H_QUEUE

#include <deque>
#include <mutex>

#include "request.h"
#include "object.h"

namespace logic
{
    class queue : public object
	{
        std::mutex queue_mutex;
    public:
		queue();
		queue(const queue& q);
		queue& operator=(const queue& q);

		~queue();
		
		//adding request to the queue
		void add(request req);
		//get number of elemets in the queue
        ull_t get_size() const { return static_cast<ull_t>(requests_in_queue.size()); }

		//getting next request
		request get_first();
		//getting last request
		request get_last();

		//if the queue has a request
        bool has_request() { return having_request_flag; }

		virtual void clean() { }

        static int cur_id;
	private:
		std::deque<request> requests_in_queue;
		bool having_request_flag;
	};

} //end namespace logic

#endif // !H_QUEUE
