#include <thread>
#include <QObject>
#include <QDebug>

namespace mymodel
{
    class request : public QObject
    {
        Q_OBJECT

    public:
        int i;
        request(int i=0) : i(i) {};
        request(const request& req) : i(req.i) {};
    };


    class generator : public QObject
    {
        Q_OBJECT

    signals:
        void request_generated(request);

    public:
        generator()
        {

        }

        void start()
        {
            std::thread th([this](){generation_thread();});
            th.detach();
            qDebug() << "generator thread created" << endl;
        }

        void generation_thread()
        {
            qDebug() << "generator thread proc" << endl;
            for(int i=0; i<30; i++)
            {
                emit request_generated(request(i));
                qDebug() << i << " generated" << endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(150));
            }
            qDebug() << "generation completed" << endl;
        }
    };

    class handler : public QObject
    {
    Q_OBJECT
    private:
        std::mutex m;

    public slots:
        void handle(request req)
        {
            std::thread([=]()
            {
                std::lock_guard<std::mutex> lock(m);
                qDebug() << req.i << " handled" << endl;
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }).detach();
        }
    };
}
