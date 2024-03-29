﻿#ifndef HOMEWIDGET_H
#define HOMEWIDGET_H

#include <QDialog>
#include <QDebug>
#include <QVBoxLayout>

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

    void on_helpButton_clicked();

    void updateLastModelsList();

signals:
    void createModel();
    void openModel();
    void openModelByPath(const QString& path); //!< Срабатывает при выборе модели из списка последних открытых
    void aboutClick();
    void exitClick();
    void helpClick();

private:
    Ui::HomeWidget *ui;
    QVBoxLayout *lastModelsLayout;
};

#endif // HOMEWIDGET_H
