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
    utility/xmlhighlighter.cpp \
    utility/modelstorage.cpp \
    utility/common.cpp \
    utility/lastmodels.cpp \
    logic/exceptions.cpp \
    logic/request_id.cpp \
    logic/request.cpp \
    logic/object.cpp \
    logic/generator.cpp \
    logic/queue.cpp \
    logic/handler.cpp \
    logic/terminator.cpp \
    logic/hub_in.cpp \
    logic/model.cpp \
    logic/link.cpp \
    interface/mainwindow.cpp \
    interface/document.cpp \
    interface/scene/modelscene.cpp \
    interface/scene/arrow.cpp \
    interface/scene/modelitem.cpp \
    interface/elementpropwindow.cpp \
    interface/scene/itemshadow.cpp \
    interface/homewidget.cpp \
    interface/createmodeldialog.cpp \
    interface/aboutdialog.cpp \
    interface/modeloptionsdialog.cpp \
    interface/optionsdialog.cpp

HEADERS  += utility/xmlhighlighter.h \
    utility/modelstorage.h \
    utility/common.h \
    utility/ifillablemodel.h \
    utility/lsfss.h \
    utility/lastmodels.h \
    logic/exceptions.h \
    logic/request_id.h \
    logic/request.h \
    logic/object.h \
    logic/generator.h \
    logic/queue.h \
    logic/handler.h \
    logic/terminator.h \
    logic/hub_in.h \
    logic/model.h \
    logic/link.h \
    interface/mainwindow.h \
    interface/document.h \
    interface/scene/modelscene.h \
    interface/scene/arrow.h \
    interface/scene/modelitem.h \
    interface/elementpropwindow.h \
    interface/scene/itemshadow.h \
    interface/homewidget.h \
    interface/createmodeldialog.h \
    interface/aboutdialog.h \
    interface/modeloptionsdialog.h \
    interface/optionsdialog.h \

FORMS    += interface/mainwindow.ui \
    interface/document.ui \
    interface/elementpropwindow.ui \
    interface/homewidget.ui \
    interface/createmodeldialog.ui \
    interface/aboutdialog.ui \
    interface/modeloptionsdialog.ui \
    interface/optionsdialog.ui

OTHER_FILES += \
    TODO.txt

RC_FILE = resources/qmb.rc

RESOURCES += \
    resources.qrc
