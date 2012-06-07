#ifndef GENERATOR_H
#define GENERATOR_H

#include "object.h"
#include "request.h"

namespace logic
{
    class model;

	//! ����� ����������. �������� ��������� ������ (����� logic::model).
    /*!
     * ������������ ����� ������, ����������� ��� ��������� ��������� (��������) �
     * ������ logic::model. �������� ��������� ����������� ��������� ���, �� ����� �����.
	 * ��������� ���������� �� ��������� ������� �������������.
     */

    class generator : public object
	{
	public:
        generator(ull_t id = 0, int period = 0, ull_t num_requests = 0, bool is_random = false, bool is_infinite = false);
		generator(const generator& gen);
        virtual ~generator();

        ull_t get_num_requests() const 													//!< ���������� ���������� ��������, ������� ��������� �������������
		{ return number_of_requests_to_generate; }	

        ull_t get_current_num_requests() const											//!< ���������� ������� ���������� ��������������� ��������
		{ return count_of_generated_requests; }

        bool is_infinite() const													    //!< ���������� ���� ����������� ��������� ��������
		{ return infinite_generating; }

		bool is_finished() const														//!< ���������� ��������� � ������������� ������ ����������
		{ return count_of_generated_requests == number_of_requests_to_generate; }

		void generate_new_request(ull_t r_id);											//!< ���������� ����� ������
		virtual request* get_request();													//!< ���������� ����������� ������� �������� ������ object
		virtual void add(request*) { }													//!< ���������� ����������� ������� �������� ������ object

	private:
		int generating_period;
		ull_t number_of_requests_to_generate;
		bool random_generating;
        bool infinite_generating;

		ull_t count_of_generated_requests;
	};

} //end namespace logic

#endif // !GENERATOR_H
