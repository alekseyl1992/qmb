#ifndef H_OBJECT
#define H_OBJECT

#include <thread>
#include <mutex>
#include <string>
#include <QObject>
#include <sstream>
#include <ctime>

#include "../utility/common.h"

namespace logic
{
    class model;

    class object : public QObject
	{
        Q_OBJECT

	public:
        object(model* p, int id);
        object(const object& obj);
        virtual ~object();

        int get_id() const { return id; }
		virtual void clean() = 0;

	protected:
        model* parent;
        int id;

        //attributes
        std::string name;
	};

}
#endif // !H_OBJECT
