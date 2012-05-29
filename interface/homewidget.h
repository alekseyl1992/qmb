#ifndef HOMEWIDGET_H
#define HOMEWIDGET_H

#include <QDialog>
#include <QDebug>

//! Пространство окон интерфейса
namespace Ui {
class HomeWidget;
}

//! Класс вкладки Домашняя страница
class HomeWidget : public QDialog
{
    Q_OBJECT
    
public:
    explicit HomeWidget(QWidget *parent = 0);
    ~HomeWidget();
    
private slots:
    void on_createModelButton_clicked();

    void on_openModelButton_clicked();

    void on_aboutButton_clicked();

    void on_exitButton_clicked();

signals:
    void createModel();
    void openModel();
    void openModelByPath(const QString& path);
    void aboutClick();
    void exitClick();

private:
    Ui::HomeWidget *ui;
};

#endif // HOMEWIDGET_H
