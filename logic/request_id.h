#ifndef H_REQUEST_ID
#define H_REQUEST_ID

#include <iostream>
#include <sstream>

#include "../utility/common.h"

namespace logic
{
    //! Структура идентификатора сообщения (запроса).
	struct request_id
	{
        request_id();
        request_id(ull_t req_gen_id, ull_t req_id);
        request_id(const request_id& req_id);

        request_id& operator= (const request_id& req_id);

        std::string txt_reqID() const;

        ull_t __req_gen_id;
        ull_t __req_id;
	};

} //end namespace logic

std::basic_ostream<char>& operator<< (std::basic_ostream<char>& os, const logic::request_id &id);

#endif // !H_REQUEST_ID
