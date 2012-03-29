/****************************************************************************
** Meta object code from reading C++ file 'model.h'
**
** Created: Sun 25. Mar 15:24:02 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../mymodel/model.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'model.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_mymodel__request[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

static const char qt_meta_stringdata_mymodel__request[] = {
    "mymodel::request\0"
};

void mymodel::request::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObjectExtraData mymodel::request::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject mymodel::request::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_mymodel__request,
      qt_meta_data_mymodel__request, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &mymodel::request::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *mymodel::request::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *mymodel::request::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_mymodel__request))
        return static_cast<void*>(const_cast< request*>(this));
    return QObject::qt_metacast(_clname);
}

int mymodel::request::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
static const uint qt_meta_data_mymodel__generator[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      20,   19,   19,   19, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_mymodel__generator[] = {
    "mymodel::generator\0\0request_generated(request)\0"
};

void mymodel::generator::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        generator *_t = static_cast<generator *>(_o);
        switch (_id) {
        case 0: _t->request_generated((*reinterpret_cast< request(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData mymodel::generator::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject mymodel::generator::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_mymodel__generator,
      qt_meta_data_mymodel__generator, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &mymodel::generator::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *mymodel::generator::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *mymodel::generator::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_mymodel__generator))
        return static_cast<void*>(const_cast< generator*>(this));
    return QObject::qt_metacast(_clname);
}

int mymodel::generator::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void mymodel::generator::request_generated(request _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
static const uint qt_meta_data_mymodel__handler[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      22,   18,   17,   17, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_mymodel__handler[] = {
    "mymodel::handler\0\0req\0handle(request)\0"
};

void mymodel::handler::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        handler *_t = static_cast<handler *>(_o);
        switch (_id) {
        case 0: _t->handle((*reinterpret_cast< request(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData mymodel::handler::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject mymodel::handler::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_mymodel__handler,
      qt_meta_data_mymodel__handler, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &mymodel::handler::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *mymodel::handler::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *mymodel::handler::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_mymodel__handler))
        return static_cast<void*>(const_cast< handler*>(this));
    return QObject::qt_metacast(_clname);
}

int mymodel::handler::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
