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
    QString path() const;
    
private slots:
    void on_pathButton_clicked();

private:
    Ui::CreateModelDialog *ui;
};

#endif // CREATEMODELDIALOG_H
