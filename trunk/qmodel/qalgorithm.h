#ifndef H_QALGORITHM
#define H_QALGORITHM

#include <thread>
#include <mutex>
#include <future>
#include <algorithm>
#include <vector>
#include "model.h"

namespace qmodel
{
	namespace qalgorithm
	{
		std::mutex m, m2;
		
		bool simulate_flag; //флаг симул€ции

		template<typename Type>
			void linking_generators_and_queues(model<Type>& Model)
			{
				typedef request_generator<Type> t_generator;
				typedef queue<Type> t_queue;
				typedef handler<Type> t_handler;
				std::for_each(Model.link_generators_queues.begin(), Model.link_generators_queues.end(), 
					[&Model](const link<t_generator*, t_queue*>& link) 
				{
					std::thread([&]()
					{
						//for(int i=1;i<=5;i++)
						for(;simulate_flag == true;)
						{
							link.lhs->generate_new_request();
							for(;;)
							{
								if (link.lhs->is_generated())
								{
									link.rhs->add(link.lhs->get_request());
									break;
								}
							}
						}
					}).detach();
				});
			}

		template<typename Type>
			void linking_queues_and_handlers(model<Type>& Model)
			{
				typedef request_generator<Type> t_generator;
				typedef queue<Type> t_queue;
				typedef handler<Type> t_handler;

				std::for_each(Model.link_queues_handlers.begin(), Model.link_queues_handlers.end(), 
					[&](const link<t_queue*, t_handler*>& link) 
					{
						std::thread([&]()
						{
							/*
								for(;;) //checking if the handler is free
								{
									if (link.rhs->is_free())
									{
										//std::lock_guard<std::mutex> lock(m2);
										link.rhs->handle(link.lhs->get_first());
										break;
									}
								}
							*/
							for(;simulate_flag == true;) //main loop for the thread
							{
								for(;;) //checking if the queue has an element
								{
									if(link.lhs->has_request())
									{
										for(;;) //checking if the handler is free
										{
											if (link.rhs->is_free())
											{
												//std::lock_guard<std::mutex> lock(m2);
												link.rhs->handle(link.lhs->get_first());
												break;
											}
										}
										break;
									}
								}
							}
						}).detach();
				});
			
			}

		template<typename Type>
			void simulation_start(model<Type>& Model)
			{
				simulate_flag = true;
				linking_generators_and_queues(Model);
				linking_queues_and_handlers(Model);
			}

		template<typename Type>
			void simulation_stop(model<Type>& Model)
			{ //останавливает выполнение 
				simulate_flag = false;
			}
	}

}
#endif // !H_QALGORITHM
