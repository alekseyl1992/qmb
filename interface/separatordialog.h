#ifndef SEPARATORDIALOG_H
#define SEPARATORDIALOG_H

#include <QDialog>

namespace Ui {
class SeparatorDialog;
}

class SeparatorDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit SeparatorDialog(QWidget *parent = 0);
    ~SeparatorDialog();
    
private:
    Ui::SeparatorDialog *ui;
};

#endif // SEPARATORDIALOG_H
