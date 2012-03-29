#ifndef H_QUEUE
#define H_QUEUE

#include <deque>
#include <QDebug>
#include "request.h"


namespace qmodel
{

template<typename Type>
class queue
{
public:
	queue() { }
	~queue() { }

	//adding request to the queue
	void add(request<Type> req);
	//get number of elemets in the queue
	int get_size() const { return static_cast<int>(requests_in_queue.size()); }
	//getting next request
	request<Type> get_first();
	//getting last request
	request<Type> get_last();

private:
	std::deque< request<Type> > requests_in_queue;
};

/**********************Implementation**********************/

//adding request to the queue
template<typename Type> inline
	void queue<Type>::add(request<Type> req) {
        qDebug() << "Reguest[" << req.get_content() << "] was put to the queue";
		requests_in_queue.push_back(req);
}

//getting next request
template<typename Type> inline
	request<Type> queue<Type>::get_first() {
		request<Type> res = *(requests_in_queue.begin());
		requests_in_queue.pop_front();
		return res;
}

//getting last request
template<typename Type> inline
	request<Type> queue<Type>::get_last() {
		request<Type> res = *(requests_in_queue.end() - 1);
		requests_in_queue.pop_back();
		return res;
}

} //end namespace qmodel

#endif // !H_QUEUE
