#include <QtGui/QApplication>
#include <QTranslator>
#include "interface/mainwindow.h"

/*! \mainpage QMB. ����������� �����
 *
 * \section intro_sec ������� ��������
 *
 * QMB - Queueing Model Builder, ���������, ��������������� ��� ��������, ��������������, ��������� � ����� ���������� ������ ��������� ������������ (���).
 * @n ����� �������: @link http://code.google.com/p/qmb/ @endlink
 *
 * \section obtain_sec ��������� �������� �����
 * ��� ��������� ���������� ������ �������� ����� ���������� ���������� ����� SVN-������.
 * ����� ���������:
 * <pre> svn checkout http://qmb.googlecode.com/svn/trunk/ qmb-read-only </pre>
 *
 * \section install_sec �������� �������
 * - ������� � ���������� Qt Creator
 * -- ������: @link http://qt.nokia.com/downloads @endlink
 * - ������� � ���������� MinGW 4.6.3
 * -- ������: @link http://sourceforge.net/projects/mingw/files/ @endlink
 * - ���������� MinGW � Qt
 * - ������� ������ � Qt Creator
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
