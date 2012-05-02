#ifndef H_HANDLER
#define H_HANDLER

#include <ostream>
#include <thread>
#include <mutex>

#include "request.h"
#include "object.h"

namespace logic
{
    class handler : public object
	{
		std::mutex handler_mutex;
	public:
		handler(int _handlePeriod = 0);
		handler(const handler& h);
		handler& operator=(const handler& h);

		~handler() { }

		//Check the busy flag of the handler
        bool is_free() { return freedom_flag; }
		//Send request to the handler
		void handle(request req);

		//gets generating period
		int get_handling_period() const { return handling_period; }
		//sets generating period
		void set_handling_period(int period) { handling_period = period; }


		virtual void clean() { }

	private:
		
		//finish the request
		void finish_handling();

		//fields
		request* cur_req;
		int handling_period;
		bool freedom_flag;

		static int cur_id;
	};

} //end namespace logic

#endif // !H_HANDLER
