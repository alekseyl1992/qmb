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
#include "logic/model.h" //TODO только из-за supportedLinks()

ModelScene::ModelScene(QMenu *itemMenu, QObject *parent)
    : QGraphicsScene(parent)
{
    myScale = 1;
    bModified = false;
    myItemMenu = itemMenu;
    myMode = MoveItem;
    myItemType = ItemType::Generator;
    line = 0;
    //textItem = 0;
    myItemColor = Qt::white;
    myTextColor = Qt::black;
    myLineColor = Qt::black;

    supportedLinks = logic::model::supportedLinks();
}

void ModelScene::addItem(ItemType itemType, QString name, int id, QPoint pos)
{
    resizeToPoint(pos);

    ModelItem *item = new ModelItem(itemType, id);
    item->setPos(pos);
    item->scaleShadow(myScale);
    item->setBrush(myItemColor);
    QGraphicsScene::addItem(item);
}

void ModelScene::addLink(ItemType fromType, int idFrom, ItemType toType, int idTo)
{
    ModelItem *startItem = nullptr;
    ModelItem *endItem = nullptr;
    //ищем item'ы по их id
    foreach(QGraphicsItem *qItem, items())
    {
        ModelItem *mItem = qgraphicsitem_cast<ModelItem *>(qItem);
        if(mItem != nullptr)
        {
            if(mItem->itemType() == fromType && mItem->id() == idFrom)
                startItem = mItem;
            else if(mItem->itemType() == toType && mItem->id() == idTo)
                endItem = mItem;
        }
    }

    if(startItem && endItem)
    {
        Arrow *arrow = new Arrow(startItem, endItem);
        arrow->setColor(myLineColor);
        startItem->addArrow(arrow);
        endItem->addArrow(arrow);
        arrow->setZValue(-1000.0);
        QGraphicsScene::addItem(arrow);
        arrow->scaleShadow(myScale);
        arrow->updatePosition();
    }
    /*else
        QMessageBox::critical(
                    (QWidget *)parent(),
                    "Ошибка",
                    QString("Произошла ошибка при добавлении связи на сцену:\n\tfromType = %1, idFrom = %2\n\ttoType = %3, idTo = %4")
                        .arg((int)fromType)
                        .arg(idFrom)
                        .arg((int)toType)
                        .arg(idTo));*/
}

void ModelScene::setMode(Mode mode)
{
    myMode = mode;
}

void ModelScene::setItemType(ItemType type)
{
    myItemType = type;
}

void ModelScene::keyPressEvent(QKeyEvent *keyEvent)
{
    if(keyEvent->key() == Qt::Key_Delete)
        removeSelectedItems();
}

void ModelScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    //Выделение по ПКМ
    if(mouseEvent->button() == Qt::RightButton)
    {
        QGraphicsItem *item = itemAt(mouseEvent->scenePos());
        if(item)
        {
            //если нажали на невыделенном элементе
            if(!item->isSelected())
                clearSelection();

            item->setSelected(true);
        }
    }

    if(mouseEvent->button() == Qt::LeftButton)
    {
        QGraphicsItem *qItem = itemAt(mouseEvent->scenePos());
        ModelItem *mItem = qgraphicsitem_cast<ModelItem *>(qItem);
        if(mItem && mItem->closeByHotStop(mouseEvent->scenePos()))
        {
            myMode = InsertLine;

            line = new QGraphicsLineItem(QLineF(mouseEvent->scenePos(),
                                        mouseEvent->scenePos()));
            line->setPen(QPen(myLineColor, 2));
            QGraphicsScene::addItem(line);
        }
    }

    QGraphicsScene::mousePressEvent(mouseEvent);
}

void ModelScene::contextMenuEvent(QGraphicsSceneContextMenuEvent *contextMenuEvent)
{
    //если что-то выбрано и мышь находиться над выбранным элементом
    QGraphicsItem *qItem = itemAt(contextMenuEvent->scenePos());
    /*qDebug() << !selectedItems().empty();
    qDebug() << qItem;
    if(qItem)
        qDebug() << qItem->isSelected();*/

    if(qItem && qItem->isSelected() && qItem->type() == ModelItem::Type)
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
    {
        QGraphicsScene::mouseMoveEvent(mouseEvent);
        //ресайз сцены при перемещаении элемента
        resizeToPoint(mouseEvent->scenePos());
    }
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
            startItems.first() != endItems.first())
        {
            ModelItem *startItem =
                qgraphicsitem_cast<ModelItem *>(startItems.first());
            ModelItem *endItem =
                qgraphicsitem_cast<ModelItem *>(endItems.first());
            //проверка связи на валидность
            if(count(supportedLinks.begin(),
                    supportedLinks.end(),
                    link(startItem->itemType(), endItem->itemType())))
            {
                Arrow *arrow = new Arrow(startItem, endItem);
                arrow->setColor(myLineColor);
                startItem->addArrow(arrow);
                endItem->addArrow(arrow);
                arrow->setZValue(-1000.0);
                QGraphicsScene::addItem(arrow);
                arrow->scaleShadow(myScale);
                arrow->updatePosition();
                bModified = true;

                emit linkInserted(startItem->itemType(), startItem->id(),
                               endItem->itemType(), endItem->id());
            }
            else
                emit wrongLink(startItem->itemType(), endItem->itemType());
        }

    }
    else if(myMode == MoveItem)
    {
        //проход по всем на случай, если была премещена сразу группа элементов

        foreach(QGraphicsItem *qItem, items())
        {
            if(qItem)
            {
                ModelItem *item = qgraphicsitem_cast<ModelItem *>(qItem);
                if(item && item->isModified())
                {
                    emit itemMoved(item->itemType(), item->id(), item->pos().toPoint());
                    bModified = true;
                    item->setModified(false);
                }
            }
        }
    }

    myMode = MoveItem;
    line = 0;
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

void ModelScene::wheelEvent(QGraphicsSceneWheelEvent *event)
{
    if(event->modifiers().testFlag(Qt::KeyboardModifier::ControlModifier))
    {
        //мастабирование окна
        double scaleFactor = 1.2;

        if(event->delta() < 0)
            scaleFactor = 1.0 / scaleFactor;
        myScale *= scaleFactor;
        //if(myScale <= 1)
        //    return;

        QGraphicsView *view = (QGraphicsView*)parent();
        QPointF fPos = event->scenePos();

        if(itemAt(fPos) != nullptr)
            view->centerOn(itemAt(fPos));
        else
            view->centerOn(fPos);


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
    //ресайз сцены при добавлении элемента
    resizeToPoint(event->scenePos());

    ModelItem *item = new ModelItem(myItemType, getFreeId(myItemType));
    item->scaleShadow(myScale);
    item->setBrush(myItemColor);
    QGraphicsScene::addItem(item);
    item->setPos(event->scenePos());
    emit itemInserted(myItemType, item->id(), item->pos().toPoint());
    myMode = Mode::MoveItem;
    bModified = true;

    event->accept();
}

void ModelScene::resizeToPoint(QPointF pos)
{
    QRectF rect = sceneRect();

    if(!rect.contains(pos))
    {
        if(pos.x() < rect.left())
        {
            rect.setLeft(pos.x());
            rect.setRight(-pos.x());
        }
        else if(pos.x() > rect.right())
        {
            rect.setRight(pos.x());
            rect.setLeft(-pos.x());
        }
        if(pos.y() < rect.top())
        {
            rect.setTop(pos.y());
            rect.setBottom(-pos.y());
        }
        else if(pos.y() > rect.bottom())
        {
            rect.setBottom(pos.y());
            rect.setTop(-pos.y());
        }

        setSceneRect(rect);
    }
}

int ModelScene::getFreeId(ItemType itemType)
{
    //формируем список занятых id
    QSet<int> ids;
    int maxId = 1;
    int minId = 0;
    foreach(QGraphicsItem *it, items())
    {
        ModelItem *modelItem = qgraphicsitem_cast<ModelItem *>(it);
        if(modelItem != nullptr)
        {
            if(modelItem->itemType() == itemType)
            {
                ids << modelItem->id();
                if(modelItem->id() > maxId)
                    maxId = modelItem->id();
                if(modelItem->id() < minId || minId == 0)
                    minId = modelItem->id();
            }
        }
    }

    //ищем свободный
    if(!ids.empty())
    {
        if(minId != 1) //если есть пропуск вначале
            return 1;

        //ищем разрыв в нумерации
        for(auto it = ids.begin(); it != ids.end()-1; ++it)
            if((*it+1) != *(it+1))
                return (*it+1);

        //не нашли, значит max
        return maxId + 1;
    }
    else //пустая сцена
        return 1;
}

void ModelScene::removeSelectedItems()
{
    foreach(QGraphicsItem *item, selectedItems())
    {
        if(item->type() == ModelItem::Type)
        {
            ModelItem *mItem = qgraphicsitem_cast<ModelItem *>(item);
            mItem->removeArrows();
            emit itemRemoved(mItem->itemType(), mItem->id());
        }
        else if(item->type() == Arrow::Type)
        {
            Arrow *arrow = qgraphicsitem_cast<Arrow *>(item);
            emit linkRemoved(arrow->startItem()->itemType(), arrow->startItem()->id(),
                             arrow->endItem()->itemType(), arrow->endItem()->id());
        }
        removeItem(item);
        bModified = true;
    }
}
