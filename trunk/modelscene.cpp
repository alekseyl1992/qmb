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

#include <QtGui>

#include "modelscene.h"
#include "arrow.h"

ModelScene::ModelScene(QMenu *itemMenu, QObject *parent)
    : QGraphicsScene(parent)
{
    myScale = 1;
    myItemMenu = itemMenu;
    myMode = MoveItem;
    myItemType = ItemType::Generator;
    line = 0;
    //textItem = 0;
    myItemColor = Qt::white;
    myTextColor = Qt::black;
    myLineColor = Qt::black;
}

void ModelScene::setMode(Mode mode)
{
    myMode = mode;
}

void ModelScene::setItemType(ItemType type)
{
    myItemType = type;
}

void ModelScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    //Выделение ПКМ только если ничего не выбрано
    if(mouseEvent->button() == Qt::RightButton && selectedItems().empty())
    {
        QGraphicsItem *item = itemAt(mouseEvent->scenePos());
        if(item)
            item->setSelected(true);
    }

    if(mouseEvent->button() != Qt::LeftButton)
        myMode = InsertLine;


    if(myMode == InsertLine)
    {
        line = new QGraphicsLineItem(QLineF(mouseEvent->scenePos(),
                                    mouseEvent->scenePos()));
        line->setPen(QPen(myLineColor, 2));
        addItem(line);
    }

    QGraphicsScene::mousePressEvent(mouseEvent);
}

void ModelScene::contextMenuEvent(QGraphicsSceneContextMenuEvent *contextMenuEvent)
{
    //если что-то выбрано и мышь находиться над выбранным элементом
    if(!selectedItems().empty() && itemAt(contextMenuEvent->scenePos()))
        myItemMenu->popup(contextMenuEvent->screenPos());
}

void ModelScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (myMode == InsertLine && line != 0)
    {
        QLineF newLine(line->line().p1(), mouseEvent->scenePos());
        line->setLine(newLine);
    }
    else if (myMode == MoveItem)
        QGraphicsScene::mouseMoveEvent(mouseEvent);
}

void ModelScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (line != 0 && myMode == InsertLine)
    {
        QList<QGraphicsItem *> startItems = items(line->line().p1());
        if (startItems.count() && startItems.first() == line)
            startItems.removeFirst();
        QList<QGraphicsItem *> endItems = items(line->line().p2());
        if (endItems.count() && endItems.first() == line)
            endItems.removeFirst();

        removeItem(line);
        delete line;

        if (startItems.count() > 0 && endItems.count() > 0 &&
            startItems.first()->type() == ModelItem::Type &&
            endItems.first()->type() == ModelItem::Type &&
            startItems.first() != endItems.first()) {
            ModelItem *startItem =
                qgraphicsitem_cast<ModelItem *>(startItems.first());
            ModelItem *endItem =
                qgraphicsitem_cast<ModelItem *>(endItems.first());
            Arrow *arrow = new Arrow(startItem, endItem);
            arrow->setColor(myLineColor);
            startItem->addArrow(arrow);
            endItem->addArrow(arrow);
            arrow->setZValue(-1000.0);
            addItem(arrow);
            arrow->scaleShadow(myScale);
            arrow->updatePosition();
        }

    }myMode = MoveItem;
    line = 0;
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

void ModelScene::wheelEvent(QGraphicsSceneWheelEvent *event)
{
    if(event->modifiers().testFlag(Qt::KeyboardModifier::ControlModifier))
    {
        //мастабирование окна
        double scaleFactor = 1.2;
        QGraphicsView *view = (QGraphicsView*)parent();
        QPointF fPos = event->scenePos();

        if(itemAt(fPos) != nullptr)
            view->centerOn(itemAt(fPos));
        else
            view->centerOn(fPos);

        if(event->delta() < 0)
            scaleFactor = 1.0 / scaleFactor;

        myScale *= scaleFactor;
        view->scale(scaleFactor, scaleFactor);
        foreach(QGraphicsItem *it, items())
        {
            ModelItem *item = qgraphicsitem_cast<ModelItem *>(it);
            if(item != nullptr)
                item->scaleShadow(myScale);
            else
            {
                Arrow *arrow = qgraphicsitem_cast<Arrow *>(it);
                if(arrow != nullptr)
                    arrow->scaleShadow(myScale);
            }
        }
    }
    else //прокрутка
        QGraphicsScene::wheelEvent(event);
}

void ModelScene::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
    event->setDropAction(Qt::CopyAction);
    event->accept();
}

void ModelScene::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
    event->setDropAction(Qt::CopyAction);
    event->accept();
}

void ModelScene::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    ModelItem *item = new ModelItem(myItemType, items().count(), myItemMenu);
    item->scaleShadow(myScale);
    item->setBrush(myItemColor);
    addItem(item);
    item->setPos(event->scenePos());
    emit itemInserted(myItemType, item->id(), item->pos().toPoint());
    myMode = Mode::MoveItem;

    event->accept();
}

