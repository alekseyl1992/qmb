#ifndef TERMINATOR_H
#define TERMINATOR_H


#include "object.h"
#include "request.h"
#include "exceptions.h"

namespace logic
{
    class object;
    class model;

    //! Класс терминатора. Является элементом logic::model.
    /*!
     * Представляет собой объект, испольуемый для удаления сообщений (запросов) из
     * модели logic::model. Завершают "жизнь" сообщения в модели.
     */
    class terminator : public object
    {
        std::mutex terminator_mutex;
    public:
        terminator(model* parent, int id=0, int period = 0);
        terminator(const terminator& t);
        terminator& operator= (const terminator& t);
        ~terminator() {
        }

        void terminate(const request& req);

        bool is_free() const { return freedom_flag; }
        ull_t get_count_of_terminated_requests() { return count_of_terminated_requests; }

        virtual void clean() { }

    private:
        request cur_req;
        int terminating_period;
        bool freedom_flag;
        ull_t count_of_terminated_requests;
    };
}
#endif // TERMINATOR_H
