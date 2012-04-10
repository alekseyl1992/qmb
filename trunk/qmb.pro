#-------------------------------------------------
#
# Project created by QtCreator 2012-03-22T23:36:49
#
#-------------------------------------------------

QT       += core gui xml
QMAKE_CXXFLAGS += -std=c++0x

TARGET = qmb
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    document.cpp \
    xmlhighlighter.cpp \
    mymodel/model.cpp \
    modelscene.cpp \
    arrow.cpp \
    modelitem.cpp \
    elementpropwindow.cpp

HEADERS  += mainwindow.h \
    model.h \
    qmodel/request_generator.h \
    qmodel/request.h \
    qmodel/queue.h \
    qmodel/model.h \
    qmodel/handler.h \
    qmodel/converter.h \
    document.h \
    xmlhighlighter.h \
    main.h \
    mymodel/model.h \
    modelscene.h \
    arrow.h \
    modelitem.h \
    qmodel/request_generator.h \
    qmodel/request.h \
    qmodel/queue.h \
    qmodel/qalgorithm.h \
    qmodel/model.h \
    qmodel/link.h \
    qmodel/handler.h \
    qmodel/exceptions.h \
    simulationlog.h \
    elementpropwindow.h \
    qmodel/request_generator.h \
    qmodel/request.h \
    qmodel/queue.h \
    qmodel/qalgorithm.h \
    qmodel/model.h \
    qmodel/link.h \
    qmodel/handler.h \
    qmodel/exceptions.h \
    qmodel/converter.h

FORMS    += mainwindow.ui \
    document.ui \
    elementpropwindow.ui
