#ifndef H_REQUEST
#define H_REQUEST

namespace qmodel
{

template<typename Type>
class request
{
public:
	request() : content(NULL) { }
	request(Type _content) : content(_content) { }
	request& operator=(const request& r) { content = r.content; return *this; }
	~request() { }

	Type get_content() const { return content; }

private:
	Type content;
};

} //end namespace qmodel

#endif // !H_REQUEST