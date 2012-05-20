#ifndef ELEMENTPROPWINDOW_H
#define ELEMENTPROPWINDOW_H

#include <QDialog>

namespace Ui {
class ElementPropWindow;
}

/*!
 * Класс окна свойств
 */
class ElementPropWindow : public QDialog
{
    Q_OBJECT
    
public:
    explicit ElementPropWindow(QWidget *parent = 0);
    ~ElementPropWindow();
    
private:
    Ui::ElementPropWindow *ui;
};

#endif // ELEMENTPROPWINDOW_H
