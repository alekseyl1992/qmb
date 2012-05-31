#ifndef LASTMODELS_H
#define LASTMODELS_H
#include <QStringList>

//! Класс предоставляет доступ к списку последних открытых моделей. Реализует паттерн singleton
class LastModels
{
private:
    const QString fileName;
    static const int maxCount = 10; //максимальное кол-во записей

    LastModels();

public:
    static LastModels& getInst();

    QStringList getList();
    void add(const QString& path);
};

#endif // LASTMODELS_H
