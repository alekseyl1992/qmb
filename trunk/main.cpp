#include <QtGui/QApplication>
#include <QTranslator>
#include "interface/mainwindow.h"

/*! \mainpage QMB. Технический отчёт
 *
 * \section intro_sec Краткое описание
 *
 * QMB - Queueing Model Builder, программа, предназначенная для создания, редактирования, симуляции и сбора статистики систем массового обслуживания (СМО).
 * @n Адрес проекта: <a href="http://code.google.com/p/qmb/">http://code.google.com/p/qmb/</a>
 * @n@n Разработчики:
 * - Леонтьев Алексей
 * - Латкин Игорь
 * - Назаров Константин
 * @n@n МГТУ имени Н.Э. Баумана, 2012 год
 * \section obtain_sec Получение исходных кодов
 * Для получения актуальной версии исходных кодов необходимо установить любой SVN-клиент.
 * Затем выполнить:
 * <pre> svn checkout http://qmb.googlecode.com/svn/trunk/ qmb-read-only </pre>
 *
 * \section install_sec Развёртка проекта
 * - Скачать и установить Qt Creator
 *   - Ссылка: <a href="http://qt.nokia.com/downloads/">http://qt.nokia.com/downloads/</a>
 * - Скачать и установить MinGW 4.6.3
 *   - Ссылка: <a href="http://sourceforge.net/projects/mingw/files/">http://sourceforge.net/projects/mingw/files/</a>
 * - Подключить MinGW к Qt
 * - Открыть проект в Qt Creator
 */

int main(int argc, char *argv[])
{
    using namespace std;
    QApplication a(argc, argv);
    QTranslator *qtTranslator = new QTranslator(&a);
    qtTranslator->load(":/translations/qt_ru.qm");
    a.installTranslator(qtTranslator);

    MainWindow w;
    w.show();

    return a.exec();
}
