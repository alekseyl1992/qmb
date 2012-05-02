#include <QtGui/QApplication>
#include <QTranslator>
#include "interface/mainwindow.h"

int main(int argc, char *argv[])
{
    using namespace std;
    QApplication a(argc, argv);
    QTranslator *qtTranslator = new QTranslator(&a);
    qtTranslator->load(":/translations/qt_ru.qm");
    a.installTranslator(qtTranslator);

    MainWindow w;
    w.showMaximized();

    return a.exec();
}
