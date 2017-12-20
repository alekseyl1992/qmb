#ifndef LASTMODELS_H
#define LASTMODELS_H
#include <QObject>
#include <QStringList>

//! Класс предоставляет доступ к списку последних открытых моделей. Реализует паттерн singleton
class LastModels : public QObject
{
Q_OBJECT
private:
    const QString fileName; //!< Имя файла со списком
    static const int maxCount = 10; //!< максимальное количество записей
    const char separator; //!< Символ разделения строк
    QStringList list; //!< Список моделей (кэш)
    bool bCashed; //!< Загружен ли список в память

    LastModels();

public:
    static LastModels& getInst();

    QStringList getList();
    void add(const QString& path);

signals:
    void changed();
};

#endif // LASTMODELS_H
