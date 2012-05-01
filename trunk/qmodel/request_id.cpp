#include "request_id.h"

namespace qmodel
{

} //end namespace qmodel

std::basic_ostream<char>& operator<< (std::basic_ostream<char>& os, const qmodel::request_id &id)
{
   //output of the id
   os << id.__req_gen_id << "-" << id.__req_id;
   return os;
}
