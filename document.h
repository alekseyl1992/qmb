#ifndef DOCUMENT_H
#define DOCUMENT_H

#include "modelscene.h"
#include <QDialog>
#include <QTextEdit>

namespace Ui {
class Document;
}

class Document : public QDialog
{
    Q_OBJECT
    
public:
    enum Tabs {Model, Code, Simulation};

    explicit Document(QWidget *parent, QMenu *menu);

    ~Document();

    ModelScene *scene();

    QTextEdit *code();
    void setActiveTab(Tabs Tab);
public slots:
    void logChanged();

private:
    Ui::Document *ui;
    ModelScene *Scene;
};

#endif // DOCUMENT_H
