#ifndef MODELOPTIONSDIALOG_H
#define MODELOPTIONSDIALOG_H

#include <QDialog>
#include "utility/modelstorage.h"

namespace Ui {
class ModelOptionsDialog;
}

//! Класс настроек модели
/*!
 * Вызывается при создании новой модели и из тулбара документа
 */
class ModelOptionsDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ModelOptionsDialog(QWidget *parent, ModelStorage *storage=0);
    ~ModelOptionsDialog();    

    //! @return Имя модели
    QString name() const;
    
private:
    Ui::ModelOptionsDialog *ui;
};

#endif // MODELOPTIONSDIALOG_H
