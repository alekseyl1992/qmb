﻿#ifndef H_EXCEPTIONS
#define H_EXCEPTIONS

#include <stdexcept>
#include <string>

namespace logic
{
	namespace exceptions
	{
        class LogicException : public std::logic_error
        {
        public:
            LogicException(const char* const & message)
                : std::logic_error(message)
            { }
            LogicException(const std::string& message)
                : std::logic_error(message)
            { }
        };

        class NoGeneratorsException : public LogicException
        {
        public:
            NoGeneratorsException()
                : LogicException("В модели не найдено генераторов")
            { }

            NoGeneratorsException(const char* const & message)
                : LogicException(message)
            { }
        };

        class NoTerminatorsException : public LogicException
        {
        public:
            NoTerminatorsException()
                : LogicException("В модели отсутствуют терминаторы")
            { }

            NoTerminatorsException(const char* const & message)
                : LogicException(message)
            { }
        };

        class NoExitPointException : public LogicException
        {
        public:
            NoExitPointException(const char* const & message)
                : LogicException(message)
            { }

            NoExitPointException(std::string message)
                : LogicException(message.c_str())
            { }
        };

        class TooBigIDException : public LogicException
        {
        public:
            TooBigIDException()
                : LogicException("Превышен лимит для идентификатора запроса. Симуляция будет прервана")
            { }
            TooBigIDException(const char* const & message)
                : LogicException(message)
            { }
        };

        class JSSepOutputNotSpecified : public LogicException
        {
        public:
            JSSepOutputNotSpecified(const std::string& sepName)
                : LogicException(std::string("В скрипте ") + sepName + " не определена переменная output")
            { }
        };

        class JSScriptError : public LogicException
        {
        public:
            JSScriptError(const std::string& sepName, const std::string& error)
                : LogicException(std::string("В скрипте ") + sepName + " произошла ошибка:\n" + error)
            { }
        };
	}
}
#endif // !H_EXCEPTIONS
