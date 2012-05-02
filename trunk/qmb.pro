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
    utility/xmlhighlighter.cpp \
    interface/scene/modelscene.cpp \
    interface/scene/arrow.cpp \
    interface/scene/modelitem.cpp \
    interface/elementpropwindow.cpp \
    interface/scene/itemshadow.cpp \
    logic/model.cpp \
    logic/link.cpp \
    logic/exceptions.cpp \
    logic/object.cpp \
    logic/queue.cpp \
    logic/generator.cpp \
    logic/handler.cpp \
    logic/request_id.cpp \
    logic/request.cpp \
    logic/object.cpp \
    interface/homewidget.cpp \
    utility/modelstorage.cpp

HEADERS  += interface/mainwindow.h \
    interface/document.h \
    utility/xmlhighlighter.h \
    interface/scene/modelscene.h \
    interface/scene/arrow.h \
    interface/scene/modelitem.h \
    utility/simulationlog.h \
    interface/elementpropwindow.h \
    utility/common.h \
    utility/modelstorage.h \
    logic/request.h \
    logic/queue.h \
    logic/object.h \
    logic/model.h \
    logic/handler.h \
    logic/generator.h \
    logic/link.h \
    logic/request_id.h \
    logic/exceptions.h \
    interface/scene/itemshadow.h \
    utility/ifillablemodel.h \
    utility/lsfss.h \
    interface/homewidget.h

FORMS    += interface/mainwindow.ui \
    interface/document.ui \
    interface/elementpropwindow.ui \
    interface/homewidget.ui

OTHER_FILES += \
    TODO.txt

RESOURCES += \
    resources.qrc
