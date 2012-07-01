#ifndef ELEMENTPROPERTIESDIALOG_H
#define ELEMENTPROPERTIESDIALOG_H

#include <QDialog>
#include <QStandardItemModel>

namespace Ui {
class ElementPropertiesDialog;
}

class ElementPropertiesDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ElementPropertiesDialog(QWidget *parent = 0);
    ~ElementPropertiesDialog();
    
private:
    Ui::ElementPropertiesDialog *ui;
    QStandardItemModel *propModel;
};

#endif // ELEMENTPROPERTIESDIALOG_H
