#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <QDialog>

namespace Ui {
class Document;
}

class Document : public QDialog
{
    Q_OBJECT
    
public:
    explicit Document(QWidget *parent = 0);
    ~Document();
    
private:
    Ui::Document *ui;
};

#endif // DOCUMENT_H
