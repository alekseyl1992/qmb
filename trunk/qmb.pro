#-------------------------------------------------
#
# Project created by QtCreator 2012-03-22T23:36:49
#
#-------------------------------------------------

QT       += core gui xml opengl
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
    itemshadow.cpp \
    qmodel/model.cpp \
    qmodel/link.cpp \
    qmodel/exceptions.cpp \
    qmodel/object.cpp \
    qmodel/queue.cpp \
    qmodel/generator.cpp \
    qmodel/handler.cpp \
    qmodel/request_id.cpp \
    qmodel/request.cpp \
    qmodel/object.cpp \
    homewidget.cpp \
    qmodel/modelstorage.cpp

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
    qmodel/object.h \
    qmodel/model.h \
    qmodel/handler.h \
    qmodel/generator.h \
    qmodel/link.h \
    qmodel/request_id.h \
    qmodel/exceptions.h \
    itemshadow.h \
    ifillablemodel.h \
    lsfss.h \
    homewidget.h

FORMS    += mainwindow.ui \
    document.ui \
    elementpropwindow.ui \
    homewidget.ui

OTHER_FILES += \
    TODO.txt \
    DeprecatedCode.cpp

RESOURCES += \
    resources.qrc
