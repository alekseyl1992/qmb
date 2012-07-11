#include "common.h"

void Unimplemented(QWidget *w)
{
    QMessageBox::critical(w, "Внимание", "Данная функция не реализована");
}

QString itemTypeToGUIString(ItemType type)
{
    switch (type)
    {
        case ItemType::Generator:
            return "Генератор";
        case ItemType::Queue:
            return "Очередь";
        case ItemType::Handler:
            return "Обработчик";
        case ItemType::Terminator:
            return "Терминатор";
        case ItemType::Collector:
            return "Собиратель";
        case ItemType::Separator:
            return "Разделитель";

    default:
        return "IllegalItemType";
    };
}

QString itemTypeToXMLString(ItemType type)
{
    switch (type)
    {
        case ItemType::Generator:
            return "Generator";
        case ItemType::Queue:
            return "Queue";
        case ItemType::Handler:
            return "Handler";
        case ItemType::Terminator:
            return "Terminator";
        case ItemType::Collector:
            return "Collector";
        case ItemType::Separator:
            return "Separator";
        default:
            return "IllegalItemType";
    };
}

qreal distance(QPointF p1, QPointF p2)
{
    return std::sqrt(pow(p1.x()-p2.x(), 2) +
                     pow(p1.y()-p2.y(), 2));
}

unsigned long long get_now_time()
{
    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
}

int getFreeId(QSet<int> ids)
{
    //ищем свободный
    if(!ids.empty())
    {
        if(!ids.contains(1)) //если есть пропуск вначале
            return 1;

        //ищем разрыв в нумерации
        for(auto it = ids.begin(); it != ids.end()-1; ++it)
            if((*it+1) != *(it+1))
                return (*it+1);

        //не нашли, значит ищем max
        int maxId = 1;
        foreach(int i, ids)
            if(i > maxId)
                maxId = i;
        return maxId + 1;
    }
    else //пустой список
        return 1;
}


