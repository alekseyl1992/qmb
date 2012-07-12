#ifndef H_REQUEST
#define H_REQUEST

#include "request_id.h"

namespace logic
{
    class request
    {
    public:
        request(int gen_id = 0, ull_t _id = 0);
        request(const request& req);
        ~request();

        request_id get_id() const { return id; }

        static ull_t get_generated_count()
        { return num_generated; }

        static ull_t get_deleted_count()
        { return num_deleted; }

    private:
        request_id id;
        static ull_t num_generated;
        static ull_t num_deleted;
    };

} //end namespace logic

#endif // !H_REQUEST
