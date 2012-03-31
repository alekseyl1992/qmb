#ifndef DOCUMENT_H
#define DOCUMENT_H

#include "modelscene.h"
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

    ModelScene *scene()
    {
        return Scene;
    }
    
private:
    Ui::Document *ui;
    ModelScene *Scene;
};

#endif // DOCUMENT_H
