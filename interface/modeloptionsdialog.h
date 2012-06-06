#ifndef MODELOPTIONSDIALOG_H
#define MODELOPTIONSDIALOG_H

#include <QDialog>

namespace Ui {
class ModelOptionsDialog;
}

class ModelOptionsDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ModelOptionsDialog(QWidget *parent = 0);
    ~ModelOptionsDialog();
    
private:
    Ui::ModelOptionsDialog *ui;
};

#endif // MODELOPTIONSDIALOG_H
