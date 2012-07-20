#ifndef PROPERTYMODEL_H
#define PROPERTYMODEL_H

#include <QAbstractTableModel>

//! Модель данных для вида свойств элементов
class PropertyModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit PropertyModel(QObject *parent = 0);
    
signals:
    
public slots:
    
};

#endif // PROPERTYMODEL_H
