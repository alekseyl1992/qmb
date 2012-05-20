/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
** $QT_END_LICENSE$
**
****************************************************************************/
#ifndef MODELSCENE_H
#define MODELSCENE_H

#include <QGraphicsScene>
#include <QWheelEvent>
#include <map>
#include "interface/scene/modelitem.h"
#include "utility/ifillablemodel.h"
#include "utility/common.h"

QT_BEGIN_NAMESPACE
class QGraphicsSceneMouseEvent;
class QMenu;
class QPointF;
class QGraphicsLineItem;
class QFont;
class QGraphicsTextItem;
class QColor;
QT_END_NAMESPACE

//! Класс Сцены
/*!
 * Представляет собой основную рабочую поверхность интерфейса программы.
 * На неё могут быть добавлены объекты классов ModelItem и Arrow
 */
class ModelScene : public QGraphicsScene, public IFillableModel
{
    Q_OBJECT

public:
    enum Mode { InsertItem, InsertLine, MoveItem };

    ModelScene(QMenu *itemMenu, QObject *parent = 0);
    qreal scale()
    {
        return myScale;
    }

    void addItem(ItemType itemType, QString name, int id, QPoint pos = QPoint());
    void addLink(ItemType fromType, int idFrom, ItemType toType, int idTo);

    void removeSelectedItems();

    //модель изменена?
    bool isModified()
    { return bModified; }
    void setModified(bool m)
    { bModified = m;}
public slots:
    void setMode(Mode mode);
    void setItemType(ItemType type);

signals:
    //сигналы для оперативного изменения XML-дерева
    void itemInserted(ItemType type, int id, QPoint pos);
    void itemMoved(ItemType type, int id, QPoint pos);
    void itemRemoved(ItemType type, int id);
    void linkInserted(ItemType fromType, int idFrom, ItemType toType, int idTo);
    void linkRemoved(ItemType fromType, int idFrom, ItemType toType, int idTo);
    void wrongLink(ItemType fromType, ItemType toType);

protected:
    void keyPressEvent(QKeyEvent *keyEvent);
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *contextMenuEvent);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void wheelEvent(QGraphicsSceneWheelEvent* event);

    //Drag'n'Drop
    void dropEvent(QGraphicsSceneDragDropEvent *event);
    void dragEnterEvent(QGraphicsSceneDragDropEvent *event);
    void dragMoveEvent(QGraphicsSceneDragDropEvent *event);

private:
    qreal myScale; //текущий масштаб сцены
    ItemType myItemType; //TODO ?
    QMenu *myItemMenu;
    Mode myMode;
    bool leftButtonDown;
    QPointF startPoint;
    QGraphicsLineItem *line;
    QFont myFont;
    QColor myTextColor;
    QColor myItemColor;
    QColor myLineColor;
    bool bModified;
    typedef std::pair<ItemType, ItemType> link;
    std::vector<link> supportedLinks;

    void resizeToPoint(QPointF pos);
    int getFreeId(ItemType itemType); //находит ближайщий пустой id
};

#endif // MODELSCENE_H
