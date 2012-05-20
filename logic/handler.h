#ifndef H_HANDLER
#define H_HANDLER

#include <ostream>

#include "request.h"
#include "object.h"


namespace logic
{
    class object;
    class model;

    //! Класс обработки сообщений. Является элементом logic::model
    /*!
     * Представляет собой объект, испольуемый для обработки сообщений (запросов) в
     * модели logic::model. Обработка происходит по абсолютному времени (периоду).
     */
    class handler : public object
	{
        friend class terminator;

        std::mutex handler_mutex, handler_mutex2;
	public:
        handler(model* parent, int id=0, int _handlePeriod = 0);
		handler(const handler& h);
		handler& operator=(const handler& h);

        ~handler() {
        }

		//gets generating period
		int get_handling_period() const { return handling_period; }
		//sets generating period
		void set_handling_period(int period) { handling_period = period; }
        //gets number of handled requests
        ull_t get_num_of_handled_requests() const { return counter_of_handled_requests; }

        //Check the busy flag of the handler
        bool is_free() { return freedom_flag; }
        bool is_handled() { return handled_flag; }
        //Send request to the handler
        void handle(const request& req);
        //pops request out
        request get_current_request();

		virtual void clean() { }

	private:

		//fields
        request cur_req;
		int handling_period;
		bool freedom_flag;
        bool handled_flag;

        ull_t counter_of_handled_requests;
	};

} //end namespace logic

#endif // !H_HANDLER
