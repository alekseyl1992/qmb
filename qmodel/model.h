#ifndef H_MODEL
#define H_MODEL

#include <vector>

#include "request.h"
#include "request_generator.h"
#include "queue.h"
#include "handler.h"
#include "link.h"

namespace qmodel
{
	//это класс, который реализует все св€зи между объектами модели
	//он же содержит списки очередей, св€занных генераторов и т.д. и т.п.
	//это входной вектор логики
	//и выходной из Converter
	

	template<typename Type = int>
	class model
	{
	public:
		typedef request_generator<Type> t_generator;
		typedef queue<Type> t_queue;
		typedef handler<Type> t_handler;

		~model() { IDs.clear(); }

		//getter - setter
		bool get_simulate_flag() const { return simulate_flag; }
		void set_simulate_flag(bool flag) { simulate_flag = flag; }

		std::vector<t_generator> req_generators;
		std::vector<t_queue> queues;
		std::vector<t_handler> handlers;

		//links
		std::vector< link <t_generator*, t_queue*> > link_generators_queues;
		std::vector< link <t_queue*, t_handler*> > link_queues_handlers;

		std::vector<std::thread::id> IDs; //all threads' ids will be kept here

	private:
		bool simulate_flag; //флаг симул€ции
	};
	
} //end namespace qmodel  

#endif // !H_MODEL