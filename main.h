#ifndef H_MAIN
#define H_MAIN

#include <iostream>
#include <thread>
#include <mutex>
#include <future>

#include "qmodel\request.h"
#include "qmodel\request_generator.h"
#include "qmodel\queue.h"
#include "qmodel\handler.h"

std::mutex m, m2;
std::promise< qmodel::request<int> >* promise;


void generating(qmodel::request_generator<int>* gen, qmodel::queue<int>* q)
{
    while(true)
    {
        std::lock_guard<std::mutex> lock(m);
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        gen->generate_new_request();
        q->add(gen->get_request());
        promise = new std::promise< qmodel::request<int> >;
        promise->set_value(q->get_first());
    }
}

void add_to_handler(qmodel::handler<int>* h)
{
    while(true)
    {
        std::lock_guard<std::mutex> lock(m);
        h->handle(promise->get_future().get());
    }
}

#endif // !H_MAIN
