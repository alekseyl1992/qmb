#ifndef H_QALGORITHM
#define H_QALGORITHM

#include <thread>
#include <algorithm>
#include <vector>
#include "model.h"

namespace qmodel
{

	namespace qalgorithm
	{
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
							Model.IDs.push_back(std::this_thread::get_id());
							for(int i=1;i<=5;i++)
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
							Model.IDs.push_back(std::this_thread::get_id());
							for(;;) //main loop for the thread
							{
								for(;;) //checking if the queue has an element
								{
									if(link.lhs->has_request())
									{
										for(;;) //checking if the handler is free
										{
											if (link.rhs->is_free())
											{
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
			void simulate_start(model<Type>& Model)
			{
				linking_generators_and_queues(Model);
			
				linking_queues_and_handlers(Model);
			}

		template<typename Type>
			void simulate_stop(model<Type>& Model)
			{
				std::for_each(Model.IDs.begin(), Model.IDs.end(), [](std::thread::id cur_id)
				{
					
				});
			}
	}

}
#endif // !H_QALGORITHM
