#ifndef CREATEMODELDIALOG_H
#define CREATEMODELDIALOG_H

#include <QDialog>

namespace Ui {
class CreateModelDialog;
}

class CreateModelDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit CreateModelDialog(QWidget *parent = 0);
    ~CreateModelDialog();

    QString name() const;
    
private slots:

private:
    Ui::CreateModelDialog *ui;
};

#endif // CREATEMODELDIALOG_H
