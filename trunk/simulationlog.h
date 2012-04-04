#ifndef SIMULATIONLOG_H
#define SIMULATIONLOG_H
#include <QTextStream>

//singleton, связь между логикой и интерфейсом
class SimulationLog : public QObject, public QTextStream
{
    Q_OBJECT

private:
    QString str;

    SimulationLog()
        : QTextStream(&str)
    {

    }
public:
    static SimulationLog& Log()
    {
        static SimulationLog log;
        return log;
    }

    SimulationLog& operator <<(QTextStream & (*fun)(QTextStream &))
    {
        (*fun)(*this);
        emit changed();
        return *this;
    }

    template<typename T>
    SimulationLog& operator <<(T some)
    {
        QTextStream::operator <<(some);

        emit changed();
        return *this;
    }

    QString& text() const
    {
        return *this->string();
    }

signals:
    void changed();
};

#define sLog SimulationLog::Log()

#endif // SIMULATIONLOG_H
