/********************************************************************************
** Form generated from reading UI file 'document.ui'
**
** Created: Sun 25. Mar 02:23:30 2012
**      by: Qt User Interface Compiler version 4.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DOCUMENT_H
#define UI_DOCUMENT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGraphicsView>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QTabWidget>
#include <QtGui/QTextEdit>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Document
{
public:
    QGridLayout *gridLayout;
    QTabWidget *tabWidget;
    QWidget *modelTab;
    QGridLayout *gridLayout_2;
    QGraphicsView *graphicsView;
    QWidget *codeTab;
    QGridLayout *gridLayout_3;
    QTextEdit *textEdit;

    void setupUi(QDialog *Document)
    {
        if (Document->objectName().isEmpty())
            Document->setObjectName(QString::fromUtf8("Document"));
        Document->resize(400, 300);
        gridLayout = new QGridLayout(Document);
        gridLayout->setContentsMargins(0, 0, 0, 0);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        tabWidget = new QTabWidget(Document);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setTabPosition(QTabWidget::South);
        tabWidget->setTabShape(QTabWidget::Triangular);
        modelTab = new QWidget();
        modelTab->setObjectName(QString::fromUtf8("modelTab"));
        gridLayout_2 = new QGridLayout(modelTab);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        graphicsView = new QGraphicsView(modelTab);
        graphicsView->setObjectName(QString::fromUtf8("graphicsView"));

        gridLayout_2->addWidget(graphicsView, 0, 0, 1, 1);

        tabWidget->addTab(modelTab, QString());
        codeTab = new QWidget();
        codeTab->setObjectName(QString::fromUtf8("codeTab"));
        gridLayout_3 = new QGridLayout(codeTab);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        textEdit = new QTextEdit(codeTab);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));

        gridLayout_3->addWidget(textEdit, 0, 0, 1, 1);

        tabWidget->addTab(codeTab, QString());

        gridLayout->addWidget(tabWidget, 0, 0, 1, 1);


        retranslateUi(Document);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(Document);
    } // setupUi

    void retranslateUi(QDialog *Document)
    {
        Document->setWindowTitle(QApplication::translate("Document", "\320\234\320\276\320\264\320\265\320\273\321\214", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(modelTab), QApplication::translate("Document", "\320\234\320\276\320\264\320\265\320\273\321\214", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(codeTab), QApplication::translate("Document", "\320\232\320\276\320\264", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Document: public Ui_Document {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DOCUMENT_H
