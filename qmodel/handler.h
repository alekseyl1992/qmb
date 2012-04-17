#ifndef H_HANDLER
#define H_HANDLER

#include <ostream>
#include <thread>
#include <mutex>

#include "request.h"
#include "qmbObject.h"

namespace qmodel
{
	class handler : public qmbObject
	{
	public:
		handler(int _handlePeriod = 0);
		handler(const handler& h);
		handler& operator=(const handler& h);

		~handler() { delete cur_req; }

		//Check the busy flag of the handler
		bool is_free() const { return freedom_flag; }
		//Send request to the handler
		void handle(request req);

		//gets generating period
		int get_handling_period() const { return handling_period; }
		//sets generating period
		void set_handling_period(int period) { handling_period = period; }

	private:
		
		//finish the request
		void finish_handling();

		//fields
		request* cur_req;
		int handling_period;
		bool freedom_flag;

		static int cur_id;
	};

} //end namespace qmodel

#endif // !H_HANDLER
