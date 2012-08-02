#ifndef H_EXCEPTIONS
#define H_EXCEPTIONS

#include <stdexcept>
#include <string>
#include <vector>
#include <stdlib.h>

#include "object.h"

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

        class IConstructableException
        {
        protected:
            std::string intToString(int val)
            {
                int digitsCount = 0;
                int tmp = val;
                while(tmp > 0)
                {
                    tmp /= 10;
                    ++digitsCount;
                }

                char* txt_num = new char[digitsCount + 1];
                itoa(val, txt_num, 10);

                std::string res(txt_num);

                delete txt_num;
                txt_num = nullptr;

                return res;
            }
            std::string ConstructIDs(std::vector<object*>& bad_objects)
            {
                std::string msgIDs = "";
                for (auto it = bad_objects.begin(); it != bad_objects.end() - 1; ++it)
                {
                    msgIDs += intToString((*it)->get_id()) + ", ";
                }
                msgIDs += intToString((*(bad_objects.end() - 1))->get_id());
                return msgIDs;
            }
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

        class NoExitPointException : public LogicException, public IConstructableException
        {
        public:
            NoExitPointException(std::vector<object*>& bad_generators)
                : LogicException(ConstructMessage(bad_generators))
            { }

            NoExitPointException(const char* const & message)
                : LogicException(message)
            { }

            std::string ConstructMessage(std::vector<object*>& bad_generators)
            {
                std::string msg = "Не для всех генераторов существует выходная точка:\n[id]";
                msg += ConstructIDs(bad_generators);
                return msg;
            }
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

        class ManyInputsOutputsException : public LogicException, public IConstructableException
        {
        public:
            ManyInputsOutputsException(std::vector<object*>& bad_inputs_objects, std::vector<object*>& bad_outputs_objects)
                : LogicException(ConstructMessage(bad_inputs_objects, bad_outputs_objects))
            { }

            ManyInputsOutputsException(const char* const & message)
                : LogicException(message)
            { }

        private:
            std::string ConstructMessage(std::vector<object*>& bad_inputs_objects, std::vector<object*>& bad_outputs_objects)
            {
                std::string msg = "";
                if (bad_inputs_objects.size() != 0)
                    msg += "У объектов со следующими id существует не один вход (используйте Собиратель):\n[id]" + ConstructIDs(bad_inputs_objects);

                if (bad_outputs_objects.size() != 0)
                    msg += "\nУ объектов со следующими id существует не один выход (используйте Разделитель):\n[id]" + ConstructIDs(bad_outputs_objects);

                return msg;
            }
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
