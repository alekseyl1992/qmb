#ifndef H_QMBOBJECT
#define H_QMBOBJECT

#include <string>

class qmbObject
{
public:
	qmbObject() { }
	qmbObject(int cur_id) : id(cur_id) { }
	qmbObject(int cur_id, char* _name): id(cur_id), name(_name) { }

	int get_id() const { return id; }

protected:
	int id;

	//properties
	std::string name;
};

#endif // !H_QMBOBJECT
