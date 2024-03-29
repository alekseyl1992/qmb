﻿/****************************************************************************
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

#ifndef ModelItem_H
#define ModelItem_H

#include <QList>
#include <QMap>
#include "utility/common.h"
#include "interface/scene/itemshadow.h"

QT_BEGIN_NAMESPACE
class QPixmap;
class QGraphicsItem;
class QGraphicsScene;
class QTextEdit;
class QGraphicsSceneMouseEvent;
class QMenu;
class QGraphicsSceneContextMenuEvent;
class QPainter;
class QStyleOptionGraphicsItem;
class QWidget;
class QPolygonF;
QT_END_NAMESPACE

class Arrow;

//! Класс элемента Сцены (ModelScene).
class ModelItem : public QGraphicsPolygonItem, public ItemShadow
{

public:
    enum { Type = UserType + 15 };

    /*!
     * Передача 0 в качестве itemId означает,
     * что item создаётся для отрисовки на панели элементов, а не в сцене
     */
    ModelItem(ItemType itemType, int itemId=0, QString name="", bool dropShadow=false,
        QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);
    ~ModelItem();

    void addArrow(Arrow *arrow);
    void removeArrow(Arrow *arrow);
    void removeArrows();
    void setScale(qreal factor);

    ItemType itemType() const
        { return myItemType; }
    QPolygonF polygon() const
        { return myPolygon; }

    QPixmap image() const;
    int type() const
        { return Type;}
    int id() const
        { return myId;}
    QString name() const
        { return myName; }
    const QList<Arrow *>& arrows()
    { return myArrows;}
    bool isModified() const
    { return bModified; }
    void setModified(bool m)
    { bModified = m; }

    bool closeByHotStop(const QPointF& pt) const; //!< близка ли точка к одному из hs
    QPointF closestHotSpot(QPointF pos) const; //!< возвращает позицию ближайшего hs

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

private:
    ItemType myItemType;
    QPolygonF myPolygon;
    QMenu *myContextMenu;
    QList<Arrow *> myArrows;
    QVector<QPointF> hotSpots; //!< точки крепления стрелок
    QString myName; //!< Отображаемое название элемента
    int myId;
    bool bModified;
};

#endif
