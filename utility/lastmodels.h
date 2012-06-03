#ifndef LASTMODELS_H
#define LASTMODELS_H
#include <QStringList>

//! Класс предоставляет доступ к списку последних открытых моделей. Реализует паттерн singleton
class LastModels : public QObject
{
Q_OBJECT
private:
    const QString fileName;
    static const int maxCount = 10; //максимальное кол-во записей
    const char separator;
    QStringList list; //!< Список моделей (кэш)
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
