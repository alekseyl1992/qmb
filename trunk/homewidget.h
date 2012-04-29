#ifndef HOMEWIDGET_H
#define HOMEWIDGET_H

#include <QDialog>

namespace Ui {
class HomeWidget;
}

class HomeWidget : public QDialog
{
    Q_OBJECT
    
public:
    explicit HomeWidget(QWidget *parent = 0);
    ~HomeWidget();
    
private slots:
    void on_createModelButton_clicked();

    void on_openModelButton_clicked();
signals:
    void createModel();
    void openModel();

private:
    Ui::HomeWidget *ui;
};

#endif // HOMEWIDGET_H
