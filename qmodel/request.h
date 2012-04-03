#ifndef H_REQUEST
#define H_REQUEST

namespace qmodel
{
	template<typename Type = int>
	class request
	{
	public:
		request() : content(static_cast<Type>(NULL)) { }
		request(Type _content) : content(_content) { }
		request& operator=(const request& r) { content = r.content; return *this; }
		~request() { }

		Type get_content() const { return content; }
	

	private:
		Type content;
	
	};

} //end namespace qmodel

#endif // !H_REQUEST