#ifndef SIMULATIONLOG_H
#define SIMULATIONLOG_H
#include <QObject>
#include <QString>
#include <string>

//singleton, связь между логикой и интерфейсом
class SimulationLog : public QObject
{
    Q_OBJECT

private:
    SimulationLog()
    {

    }
public:
    static SimulationLog& Log()
    {
        static SimulationLog log;
        return log;
    }

    /*SimulationLog& operator <<(QTextStream & (*fun)(QTextStream &))
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
    }*/

    //thread-safe writing
    void writeLine(std::string line)
    {
        emit changed(QString(line.c_str()));
    }

    void clear()
    {
        emit cleared();
    }

signals:
    void changed(QString);
    void cleared();
};

#define sLog SimulationLog::Log()

#endif // SIMULATIONLOG_H
