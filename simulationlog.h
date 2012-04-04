#ifndef SIMULATIONLOG_H
#define SIMULATIONLOG_H
#include <QString>
#include <QObject>
#include <sstream>

//singleton, связь между логикой и интерфейсом
class SimulationLog : public QObject
{
    Q_OBJECT

private:
    std::stringstream ss;
    SimulationLog() {}
public:
    static SimulationLog& Log()
    {
        static SimulationLog log;
        return log;
    }

    template<typename T>
    SimulationLog& operator <<(T some)
    {
        ss << some;
        emit changed();
        return *this;
    }

    QString text() const
    {
        return ss.str().c_str();
    }

    void clear()
    {
       ss.clear();
    }

signals:
    void changed();
};

#define sLog SimulationLog::Log()

#endif // SIMULATIONLOG_H
