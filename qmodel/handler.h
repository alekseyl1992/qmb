#ifndef H_HANDLER
#define H_HANDLER

#include "request.h"
#include <thread>
#include <QDebug>

namespace qmodel
{

template<typename Type>
class handler
{
public:
	handler() : busy_flag(false), cur_req(NULL) { }
	~handler() { delete cur_req; }

	//Check the busy flag of the handler
	bool is_busy() const { return busy_flag; }
	//Send request to handler
	void handle(request<Type> req);

private:
	//finish the request
	void finish_hadling();

	//field
	bool busy_flag;
	request<Type>* cur_req;

};

/**********************Implementation**********************/

//Send request to handler
template<typename Type> inline
	void handler<Type>::handle(request<Type> req) {
        qDebug() << "Reguest[" << req.get_content() << "] was put to the handler";
		busy_flag = true;
		cur_req = &req;
		std::this_thread::sleep_for(std::chrono::milliseconds(5000));
		finish_hadling();
}

//finish the request
template<typename Type> inline
	void handler<Type>::finish_hadling() {
		busy_flag = false;
        qDebug() << "Request[" << cur_req->get_content() << "] finished handling";
}

} //end namespace qmodel

#endif // !H_HANDLER
