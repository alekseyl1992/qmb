#ifndef H_REQUEST_GENERATOR
#define H_REQUEST_GENERATOR

#include "request.h"
#include <QDebug>

namespace qmodel
{

template<typename Type>
class request_generator
{
public:
	request_generator() { new_req = NULL; }
	~request_generator() { delete new_req; }

	//Function generates new request
	void generate_request(Type content);
	//generating new random request
	void generate_new_request();
	//gets next request 
	request<Type> get_request();

private:
	//Fields
	request<Type> *new_req;
	static int count;
};


/**********************Implementation**********************/

template<typename Type> int request_generator<Type>::count = 0;

//generating new request
template<typename Type> inline
	void request_generator<Type>::generate_request(Type content) {
		new_req = new request<Type>(content);
}

//generating new random request
template<typename Type> inline
	void request_generator<Type>::generate_new_request() {
		generate_request(++count);
        qDebug() << "Reguest[" << count << "] generated";
}

//gets next request 
template<typename Type> inline
	request<Type> request_generator<Type>::get_request() {
		request<Type> res = *new_req;
		new_req = NULL;
		return res;
}

} //end namespace qmodel

#endif // !H_REQUEST_GENERATOR
