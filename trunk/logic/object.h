#ifndef H_OBJECT
#define H_OBJECT

#include "../utility/common.h"

#include <string>
#include <QObject>
#include <sstream>

namespace logic
{
    class object : public QObject
	{
        Q_OBJECT

	public:
        object() { }
        object(int cur_id) : id(cur_id) { }
        object(int cur_id, char* _name): id(cur_id), name(_name) { }

		int get_id() const { return id; }
		virtual void clean() = 0;
	protected:
		int id;

		//global properties
		std::string name;
	};

}
#endif // !H_OBJECT
