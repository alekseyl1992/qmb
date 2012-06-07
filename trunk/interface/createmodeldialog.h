#ifndef CREATEMODELDIALOG_H
#define CREATEMODELDIALOG_H

#include <QDialog>

namespace Ui {
class CreateModelDialog;
}

//! Класс окна с запросом ввода имени модели
/*!
 * @deprecated Вместо него теперь используется ModelOptionsDialog
 */
class CreateModelDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit CreateModelDialog(QWidget *parent = 0);
    ~CreateModelDialog();

    //! @return Имя модели
    QString name() const;
    
private slots:

private:
    Ui::CreateModelDialog *ui;
};

#endif // CREATEMODELDIALOG_H
