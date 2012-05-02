#include "lsfss.h"

bool connect(QObject *sender, const char *signal, const std::function<void()> &reciever, Qt::ConnectionType type) {
    return QObject::connect(sender, signal, new connect_functor_helper(sender, reciever), SLOT(signaled()), type);
}
