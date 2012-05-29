#include "elementpropwindow.h"
#include "ui_elementpropwindow.h"

ElementPropWindow::ElementPropWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ElementPropWindow)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Tool);
    activateWindow();
}

ElementPropWindow::~ElementPropWindow()
{
    delete ui;
}
