#ifndef H_MODEL
#define H_MODEL

#include <vector>
#include <algorithm>
#include <thread>

#include "request.h"
#include "request_generator.h"
#include "queue.h"
#include "handler.h"
#include "link.h"

namespace qmodel
{
	//��� �����, ������� ��������� ��� ����� ����� ��������� ������
	//�� �� �������� ������ ��������, ��������� ����������� � �.�. � �.�.
	//��� ������� ������ ������
	//� �������� �� Converter
	

	template<typename Type = int>
	class model
	{
	public:
		typedef request_generator<Type> t_generator;
		typedef queue<Type> t_queue;
		typedef handler<Type> t_handler;

		~model() { IDs.clear(); }

		std::vector<t_generator> req_generators;
		std::vector<t_queue> queues;
		std::vector<t_handler> handlers;

		//links
		std::vector< link <t_generator*, t_queue*> > link_generators_queues;
		std::vector< link <t_queue*, t_handler*> > link_queues_handlers;

		std::vector<std::thread::id> IDs; //all threads' id will be kept here
	};
	
} //end namespace qmodel  

#endif // !H_MODEL