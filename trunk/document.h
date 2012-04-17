#ifndef DOCUMENT_H
#define DOCUMENT_H

#include "modelscene.h"
#include "qmodel/modelstorage.h"
#include <QDialog>
#include <QTextEdit>
#include <QTreeWidgetItem>

namespace Ui {
class Document;
}

//class ModelStorage; //forward declaration

class Document : public QDialog
{
    Q_OBJECT
    
public:
    enum Tabs {Model, Code, Simulation};

    explicit Document(QWidget *parent, QMenu *menu, QString name);

    ~Document();

    ModelScene *scene();

    QTextEdit *code();
    void setActiveTab(Tabs Tab);
public slots:
    void logChanged();

private slots:


    void on_logButton_toggled(bool checked);

private:
    Ui::Document *ui;
    ModelScene *Scene;
    qmodel::ModelStorage *Storage;
};

#endif // DOCUMENT_H
