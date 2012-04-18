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
    modelscene.cpp \
    arrow.cpp \
    modelitem.cpp \
    elementpropwindow.cpp \
    qmodel/request_id.cpp \
    qmodel/request.cpp \
    qmodel/queue.cpp \
    qmodel/qmbObject.cpp \
    qmodel/model.cpp \
    qmodel/link.cpp \
    qmodel/handler.cpp \
    qmodel/generator.cpp \
    qmodel/exceptions.cpp

HEADERS  += mainwindow.h \
    document.h \
    xmlhighlighter.h \
    main.h \
    modelscene.h \
    arrow.h \
    modelitem.h \
    simulationlog.h \
    elementpropwindow.h \
    common.h \
    qmodel/modelstorage.h \
    qmodel/request.h \
    qmodel/queue.h \
    qmodel/qmbObject.h \
    qmodel/model.h \
    qmodel/handler.h \
    qmodel/generator.h \
    qmodel/link.h \
    qmodel/request_id.h \
    qmodel/exceptions.h

FORMS    += mainwindow.ui \
    document.ui \
    elementpropwindow.ui

OTHER_FILES += \
    TODO.txt
