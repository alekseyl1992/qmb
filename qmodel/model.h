#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <utility>
#include "request.h"
#include "request_generator.h"
#include "queue.h"
#include "handler.h"

namespace qmodel
{
//��� �����, ������� ��������� ��� ����� ����� ��������� ������
//�� �� �������� ������ ��������, ��������� ����������� � �.�. � �.�.
//��� ������� ������ ������
//� �������� �� Converter
template<typename T>
class model
{
private:
    std::vector< request_generator<T> > req_generators;
    std::vector< queue<T> > req_queues;
    std::vector< handler<T> > req_handlers;

    //links
    std::vector< std::pair< request_generator<T>*, queue<T>* > > generators_queues_link;
    std::vector< std::pair< queue<T>*, handler<T>* > > queues_handlers_link;
    std::vector< std::pair< request_generator<T>*, handler<T>* > > generators_handlers_link;
};

} //end namespace qmodel
#endif // QMODEL_H
