#include "separator.h"
#include "exceptions.h"
#include <time.h>

namespace logic
{
    separator::separator(std::string name, int id, QString script)
        : object(ItemType::Separator, name, id),
          engine(new QScriptEngine()),
          script(script), some(3)
    { }

    separator::separator(separator &sep) :
        object(sep), engine(new QScriptEngine()), script(sep.script)
    { }

    separator::~separator()
    {
        delete engine;
    }

    object* separator::find_way()
    {
        //TODO где-то нужно (в Validator) реализовать проверку скриптов на ошибки

        engine->setGlobalObject(engine->newQObject(this));
        engine->globalObject().setProperty("time", QScriptValue(get_event_time()));
        engine->globalObject().setProperty("outputsCount", QScriptValue(outputs.size()));

        QScriptValue returnValue = engine->evaluate(script);
        if(returnValue.isError())
            throw exceptions::JSScriptError(name, returnValue.toString().toStdString());

        QScriptValue sOutput = engine->globalObject().property("output");

        std::list<object*>::iterator it;

        if(sOutput.isNumber()) //id указан
        {
            //теперь нужно преобразовать id элемента в его id в списке outputs
            int outputID = sOutput.toNumber();
            it = std::find_if(outputs.begin(), outputs.end(), [outputID](const object *obj)
            {
                return obj->get_id() == outputID;
            });
        }
        else if(sOutput.isString()) //указано имя
        {
            //теперь нужно преобразовать id элемента в его id в списке outputs
            std::string outputName = sOutput.toString().toStdString();
            it = std::find_if(outputs.begin(), outputs.end(), [outputName](const object *obj)
            {
                return obj->get_name() == outputName;
            });
        }
        else //родила царица в ночь, ни то сына, ни то дочь..
            throw exceptions::JSSepOutputNotSpecified(name);

        return *it;
    }

    request* separator::get_request()
    {
        std::lock_guard<std::mutex> lk(item_mutex);

        moveable_request_flag = false;
        freedom_flag = true;
        return cur_req;
    }

    void separator::add(request* req)
    {
        std::lock_guard<std::mutex> lk(item_mutex);

        freedom_flag = false;
        cur_req = req;
        moveable_request_flag = true;

        qDebug() << cur_req->get_id().str_reqID().c_str() << "is in Separator " << get_id();
    }

    void separator::move_request()
    {
        //вытаскиваем запрос из input()
        if (input()->is_moveable() && this->is_free() &&
                 input()->get_type() != ItemType::Separator)
        {
            this->add(input()->get_request());
        }

        object* way = find_way();

        //помещаем запрос в один из выходов в случае если на выходе НЕ коллектор
        if (this->is_moveable() && way->is_free() &&
                way->get_type() != ItemType::Collector)
        {
            way->add(this->get_request());
        }
    }

    int separator::randID()
    {
        std::list<object*>::iterator obegin = outputs.begin();
        advance(obegin, rand() % outputs.size());
        return (*obegin)->get_id(); //случайный выход
    }

} //end namespace logic
