#ifndef LSFSS_H
#define LSFSS_H

#include <QObject>
#include <functional>

class connector : public QObject {
    Q_OBJECT
public:
    connector(QObject *parent, const std::function<void()> &f) : QObject(parent), function_(f) {
    }

public Q_SLOTS:
    void signaled() {
        function_();
    }

private:
    std::function<void()> function_;
};

bool connect(QObject *sender, const char *signal, const std::function<void()> &reciever, Qt::ConnectionType type = Qt::AutoConnection);

#endif
