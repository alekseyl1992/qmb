#ifndef OPTIONSDIALOG_H
#define OPTIONSDIALOG_H

#include <QDialog>

namespace Ui {
class OptionsDialog;
}

//! Окно настроек QMB
class OptionsDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit OptionsDialog(QWidget *parent = 0);
    ~OptionsDialog();
    
private slots:
    void onAccepted(); //!< Срабатывает при нажатии ОК

private:
    Ui::OptionsDialog *ui;
};

#endif // OPTIONSDIALOG_H
