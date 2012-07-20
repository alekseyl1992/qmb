#-------------------------------------------------
#
# Project created by QtCreator 2012-03-22T23:36:49
#
#-------------------------------------------------

QT       += core gui xml opengl script
QMAKE_CXXFLAGS += -std=c++0x

TARGET = qmb
TEMPLATE = app

SOURCES += main.cpp \
    utility/xmlhighlighter.cpp \
    utility/modelstorage.cpp \
    utility/common.cpp \
    utility/lastmodels.cpp \
    utility/lsfss.cpp \
    utility/validator.cpp \
    logic/exceptions.cpp \
    logic/attribute.cpp \
    logic/request_id.cpp \
    logic/request.cpp \
    logic/object.cpp \
    logic/generator.cpp \
    logic/queue.cpp \
    logic/handler.cpp \
    logic/terminator.cpp \
    logic/collector.cpp \
    logic/separator.cpp \
    logic/model.cpp \
    logic/link.cpp \
    interface/mainwindow.cpp \
    interface/document.cpp \
    interface/scene/modelscene.cpp \
    interface/scene/arrow.cpp \
    interface/scene/modelitem.cpp \
    interface/scene/itemshadow.cpp \
    interface/homewidget.cpp \
    interface/createmodeldialog.cpp \
    interface/aboutdialog.cpp \
    interface/modeloptionsdialog.cpp \
    interface/optionsdialog.cpp \
    interface/elementpropertiesdialog.cpp \
    interface/separatordialog.cpp \
    interface/propertymodel.cpp


HEADERS  += utility/xmlhighlighter.h \
    utility/modelstorage.h \
    utility/common.h \
    utility/ifillablemodel.h \
    utility/lsfss.h \
    utility/lastmodels.h \
    utility/validator.h \
    logic/exceptions.h \
    logic/attribute.h \
    logic/request_id.h \
    logic/request.h \
    logic/object.h \
    logic/generator.h \
    logic/queue.h \
    logic/handler.h \
    logic/terminator.h \
    logic/collector.h \
    logic/separator.h \
    logic/model.h \
    logic/link.h \
    interface/mainwindow.h \
    interface/document.h \
    interface/scene/modelscene.h \
    interface/scene/arrow.h \
    interface/scene/modelitem.h \
    interface/scene/itemshadow.h \
    interface/homewidget.h \
    interface/createmodeldialog.h \
    interface/aboutdialog.h \
    interface/modeloptionsdialog.h \
    interface/optionsdialog.h \
    interface/elementpropertiesdialog.h \
    interface/separatordialog.h \
    interface/propertymodel.h


FORMS    += interface/mainwindow.ui \
    interface/document.ui \
    interface/homewidget.ui \
    interface/createmodeldialog.ui \
    interface/aboutdialog.ui \
    interface/modeloptionsdialog.ui \
    interface/optionsdialog.ui \
    interface/elementpropertiesdialog.ui \
    interface/separatordialog.ui

OTHER_FILES += \
    TODO.txt

RC_FILE = resources/qmb.rc

RESOURCES += \
    resources.qrc
