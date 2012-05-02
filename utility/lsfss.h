#ifndef LSFSS_H
#define LSFSS_H

#include <QObject>
#include <functional>

class connect_functor_helper : public QObject {
    Q_OBJECT
public:
    connect_functor_helper(QObject *parent, const std::function<void()> &f) : QObject(parent), function_(f) {
    }

public Q_SLOTS:
    void signaled() {
        function_();
    }

private:
    std::function<void()> function_;
};

/*static*/ bool connect(QObject *sender, const char *signal, const std::function<void()> &reciever, Qt::ConnectionType type = Qt::AutoConnection);

#endif
