#ifndef H_REQUEST_ID
#define H_REQUEST_ID

#include <string>

typedef unsigned long long ull_t;

namespace logic
{
    //! Класс идентификатора сообщения (запроса).

    class request_id
    {
    public:
        request_id();
        request_id(ull_t req_gen_id, ull_t req_id);
        request_id(const request_id& req_id);

        std::string str_reqID() const;             //!< Возвращает строковое представление id

    private:
        ull_t __req_gen_id;
        ull_t __req_id;
    };

} //end namespace logic

#endif // !H_REQUEST_ID
