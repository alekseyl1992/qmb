#include "separator.h"
#include "exceptions.h"
#include <time.h>
#include <algorithm>

namespace logic
{
    separator::separator(int id, QString script)
        : object(ItemType::Separator, id),
          engine(new QScriptEngine()),
          script(script)
    { }

    separator::separator(separator &sep) :
        object(sep), engine(new QScriptEngine()), script(sep.script)
    { }

    separator::~separator()
    {
        delete engine;
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

        //TODO где-то нужно (в Validator) реализовать проверку скриптов на ошибки

        //ниже - три строки маразма, они призваны сократить реализацию рандомного выбора выхода
        //но заччем этот кусок кода, если он может и вовсе не использоваться?
        //надо бы оформить его ввиде JS + C++ функции
        std::list<object*>::iterator obegin = outputs.begin();
        advance(obegin, rand()%outputs.size());
        int randID = (*obegin)->get_id(); //случайный выход

        engine->globalObject().setProperty("time", QScriptValue(get_event_time()));
        engine->globalObject().setProperty("randID", QScriptValue(randID));
        engine->globalObject().setProperty("outputsCount", QScriptValue(outputs.size()));

        QScriptValue returnValue = engine->evaluate(script);
        if(returnValue.isError())
            throw exceptions::JSScriptError(name, returnValue.toString().toStdString());

        QScriptValue sOutputID = engine->globalObject().property("outputID");
        QScriptValue sOutputName = engine->globalObject().property("outputName");

        std::list<object*>::iterator it;

        if(!sOutputID.isUndefined()) //id указан
        {
            //теперь нужно преобразовать id элемента в его id в списке outputs
            int outputID = sOutputID.toNumber();
            it = std::find_if(outputs.begin(), outputs.end(), [outputID](const object *obj)
            {
                return obj->get_id() == outputID;
            });
        }
        else if(!sOutputName.isUndefined()) //указано имя
        {
            //теперь нужно преобразовать id элемента в его id в списке outputs
            std::string outputName = sOutputName.toString().toStdString();
            it = std::find_if(outputs.begin(), outputs.end(), [outputName](const object *obj)
            {
                return obj->get_name() == outputName;
            });
        }
        else
            throw exceptions::JSSepOutputNotSpecified(name);

        //помещаем запрос в один из выходов в случае если на выходе НЕ коллектор
        if (this->is_moveable() && (*it)->is_free() &&
                (*it)->get_type() != ItemType::Collector)
        {
            (*it)->add(this->get_request());
        }
    }

    bool separator::is_completed()
    {
        return is_free();
    }



} //end namespace logic
