#-------------------------------------------------
#
# Project created by QtCreator 2012-03-22T23:36:49
#
#-------------------------------------------------

QT       += core gui xml opengl
QMAKE_CXXFLAGS += -std=c++0x

TARGET = qmb
TEMPLATE = app

SOURCES += main.cpp \
    interface/mainwindow.cpp \
    interface/document.cpp \
    interface/scene/modelscene.cpp \
    interface/scene/arrow.cpp \
    interface/scene/modelitem.cpp \
    interface/elementpropwindow.cpp \
    interface/scene/itemshadow.cpp \
    interface/homewidget.cpp \
    utility/xmlhighlighter.cpp \
    utility/modelstorage.cpp \
    logic/exceptions.cpp \
    logic/link.cpp \
    logic/object.cpp \
    logic/request_id.cpp \
    logic/request.cpp \
    logic/generator.cpp \
    logic/queue.cpp \
    logic/handler.cpp \
    logic/model.cpp \
    utility/lsfss.cpp \
    logic/terminator.cpp \
    interface/createmodeldialog.cpp

HEADERS  += utility/xmlhighlighter.h \
    utility/simulationlog.h \
    utility/modelstorage.h \
    utility/common.h \
    utility/ifillablemodel.h \
    utility/lsfss.h \
    interface/mainwindow.h \
    interface/document.h \
    interface/scene/modelscene.h \
    interface/scene/arrow.h \
    interface/scene/modelitem.h \
    interface/elementpropwindow.h \
    interface/scene/itemshadow.h \
    interface/homewidget.h \
    logic/exceptions.h \
    logic/link.h \
    logic/object.h \
    logic/request_id.h \
    logic/request.h \
    logic/generator.h \
    logic/queue.h \
    logic/handler.h \
    logic/model.h \
    logic/terminator.h \
    interface/createmodeldialog.h

FORMS    += interface/mainwindow.ui \
    interface/document.ui \
    interface/elementpropwindow.ui \
    interface/homewidget.ui \
    interface/createmodeldialog.ui

OTHER_FILES += \
    TODO.txt

RESOURCES += \
    resources.qrc
