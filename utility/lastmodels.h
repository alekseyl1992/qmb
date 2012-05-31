#ifndef LASTMODELS_H
#define LASTMODELS_H
#include <QStringList>

//! ����� ������������� ������ � ������ ��������� �������� �������. ��������� ������� singleton
class LastModels
{
private:
    const QString fileName;
    static const int maxCount = 10; //������������ ���-�� �������
    const char separator;

    LastModels();

public:
    static LastModels& getInst();

    QStringList getList();
    void add(const QString& path);
};

#endif // LASTMODELS_H
