#ifndef H_EXCEPTIONS
#define H_EXCEPTIONS

//! Перечисление ошибок модели
enum error_code {
    INPUT_IN_GENERATOR,
    OUTPUT_IN_TERMINATOR,
    NO_GENERATORS,
    NO_TERMINATORS,
    N
};
const char* error_code_str[error_code::N] = {
    "INPUT_IN_GENERATOR",
    "OUTPUT_IN_TERMINATOR",
    "NO_GENERATORS",
    "NO_TERMINATORS"
};

namespace logic
{
	namespace exceptions
	{
		class no_requests
		{ }; 

		class too_big_id
		{ };
	}
}
#endif // !H_EXCEPTIONS
