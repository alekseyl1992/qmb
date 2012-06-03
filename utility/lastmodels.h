#ifndef LASTMODELS_H
#define LASTMODELS_H
#include <QStringList>

//! ����� ������������� ������ � ������ ��������� �������� �������. ��������� ������� singleton
class LastModels : public QObject
{
Q_OBJECT
private:
    const QString fileName;
    static const int maxCount = 10; //������������ ���-�� �������
    const char separator;
    QStringList list; //!< ������ ������� (���)
    bool bCashed;

    LastModels();

public:
    static LastModels& getInst();

    QStringList getList();
    void add(const QString& path);

signals:
    void changed();
};

#endif // LASTMODELS_H
