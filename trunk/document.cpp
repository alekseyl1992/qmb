#include "document.h"
#include "ui_document.h"
#include "xmlhighlighter.h"
#include "simulationlog.h"
#include "common.h"
#include <QMenu>
#include <QComboBox>
#include <QGraphicsOpacityEffect>

Document::Document(QWidget *parent, QMenu *menu, QString name) :
    QDialog(parent),
    ui(new Ui::Document)
{
    ui->setupUi(this);
    setWindowTitle(name);

    new XmlHighlighter(ui->textEdit);
    Scene = new ModelScene(menu, ui->graphicsView);
    ui->graphicsView->setScene(Scene);
    Storage = new qmodel::ModelStorage(name);

    //���������� ����� � ����������
    connect(Scene, SIGNAL(itemInserted(ItemType, int, QPoint)),
            Storage, SLOT(onItemInserted(ItemType, int, QPoint)));
    connect(Scene, SIGNAL(itemMoved(int, QPoint)),
            Storage, SLOT(onItemMoved(int, QPoint)));
    connect(Scene, SIGNAL(itemRemoved(int)),
            Storage, SLOT(onItemRemoved(int)));

    //������ ������ ��� ����������� �������� ������
    QComboBox *box = new QComboBox(ui->graphicsView);
    box->addItem(QIcon(), "100%");
    QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect(this);
    effect->setOpacity(0.5);
    box->setGraphicsEffect(effect);

    //������������� ������ � ������ sLog � �������������� ���� � ����������
    connect(&sLog, SIGNAL(changed()), this, SLOT(logChanged()));
}

Document::~Document()
{
    //����� � ������� �������� �����
    delete Storage;
    delete ui;
}

ModelScene *Document::scene()
{
    return Scene;
}

QTextEdit *Document::code()
{
    return ui->textEdit;
}

void Document::setActiveTab(Document::Tabs Tab)
{
    ui->tabWidget->setCurrentIndex(Tab);
}

void Document::logChanged()
{
    //����� ���������� ������ ������, �� ����������
    ui->simulationLog->setText(SimulationLog::Log().text());
}
