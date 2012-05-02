#include "request_id.h"

namespace logic
{

} //end namespace logic

std::basic_ostream<char>& operator<< (std::basic_ostream<char>& os, const logic::request_id &id)
{
   //output of the id
   os << id.__req_gen_id << "-" << id.__req_id;
   return os;
}
