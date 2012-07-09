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
#include <QGraphicsDropShadowEffect>
#include "modelitem.h"
#include "arrow.h"
#include "modelscene.h"

ModelItem::ModelItem(ItemType itemType, int itemId, QString name, bool dropShadow,
             QGraphicsItem *parent, QGraphicsScene *scene)
    : QGraphicsPolygonItem(parent, scene), ItemShadow(this, dropShadow)
{
    myItemType = itemType;
    myId = itemId;
    myName = name;
    QPen iPen = pen();
    iPen.setWidth(2);
    setPen(iPen);
    bModified = false;
    qreal dhs = 0; //отступ hotspot

    QPainterPath path;
    QRectF itemRect(0, 0, 200, 100);
    switch (myItemType) {
        case ItemType::Generator:
            path.addRect(itemRect);
            path.addPolygon(QPolygonF() << QPointF(200, 0) << QPointF(230, 50) << QPointF(200, 100));
            path = path.simplified();
            hotSpots << QPoint(230+dhs, 50);
            break;

        case ItemType::Queue:
            path.addRect(itemRect);
            hotSpots << QPoint(itemRect.left()-dhs,
                                (itemRect.bottom()-itemRect.top())/2);
            hotSpots << QPoint(itemRect.right()+dhs,
                                (itemRect.bottom()-itemRect.top())/2);
            break;

        case ItemType::Handler:
            path.addRoundedRect(itemRect, 25, 25);
            hotSpots << QPoint(itemRect.left()-dhs,
                                (itemRect.bottom()-itemRect.top())/2);
            hotSpots << QPoint(itemRect.right()+dhs,
                                (itemRect.bottom()-itemRect.top())/2);
            break;

        case ItemType::Terminator:
            path.addRect(itemRect.translated(30, 0));
            path.addPolygon(QPolygonF() << QPointF(30, 0) << QPointF(0, 50) << QPointF(30, 100));
            path = path.simplified();
            hotSpots << QPoint(0-dhs, 50);
            break;

        case ItemType::Collector:
            path.addPolygon(QPolygonF() << QPointF(0, 0) << QPointF(30, 50) << QPointF(0, 100));
            hotSpots << QPoint(30+dhs, 50);
            hotSpots << QPoint(0-dhs, 25);
            hotSpots << QPoint(0-dhs, 75);
            break;

        case ItemType::Separator:
            path.addPolygon(QPolygonF() << QPointF(30, 0) << QPointF(0, 50) << QPointF(30, 100));
            hotSpots << QPoint(0-dhs, 50);
            hotSpots << QPoint(30+dhs, 25);
            hotSpots << QPoint(30+dhs, 75);
            break;
        default:
            break;
    }

    itemRect = path.boundingRect();
    path.translate(-itemRect.width()/2, -itemRect.height()/2);
    myPolygon = path.toFillPolygon();
    setPolygon(myPolygon);

    //уменьшаем, если не для палитры
    if(itemId)
        for(QPointF &pnt : myPolygon)
            pnt /= 2;

    //перемещаем hotSpot'ы
    //for(auto it = hotSpots.begin(); it!= hotSpots.end(); ++it)
    for(QPointF &pnt : hotSpots)
    {
        pnt += QPointF(-itemRect.width()/2, -itemRect.height()/2);
        pnt /= 2;
    }

    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
}

ModelItem::~ModelItem()
{

}

void ModelItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    //устанавливаем градиентную заливку
    QLinearGradient linearGrad(QPointF(0, 0),
                               QPointF(0, boundingRect().height()));
    linearGrad.setColorAt(0, QColor(250, 250, 230));
    linearGrad.setColorAt(1, QColor(210, 210, 190));
    painter->setBrush(QBrush(linearGrad));
    QRectF rect = boundingRect();
    //rect.setTop(rect.bottom() - fract * rect.height());
    painter->drawPolygon(polygon().intersected(rect), fillRule());
    if(isSelected())
    {
        QBrush brush = QBrush(Qt::black, Qt::Dense6Pattern);
        qreal scale = ((ModelScene *)scene())->scale();
        QTransform transform;
        transform.scale(1./scale, 1./scale);
        brush.setTransform(transform);
        painter->setBrush(brush);

        painter->drawPolygon(polygon().intersected(rect), fillRule());
    }

    //рамочка
    painter->drawPolygon(polygon(), fillRule());

    //текст
    if(itemType() != ItemType::Collector && itemType() != ItemType::Separator)
    {
        painter->setPen(QPen(Qt::black));
        painter->setBrush(QBrush(Qt::black));
        painter->drawText(polygon().boundingRect(),
                          myName,
                          QTextOption(Qt::AlignCenter));
    }

    //hotSpots
    QPen hsPen;
    hsPen.setWidth(1);
    QBrush hsBrush(Qt::white);

    painter->setPen(hsPen);
    painter->setBrush(hsBrush);
    for(QPointF &pt: hotSpots)
        painter->drawEllipse(pt, 2, 2);
}

void ModelItem::removeArrow(Arrow *arrow)
{
    int index = myArrows.indexOf(arrow);

    if (index != -1)
        myArrows.removeAt(index);
}

void ModelItem::removeArrows()
{
    foreach (Arrow *arrow, myArrows) {
        arrow->startItem()->removeArrow(arrow);
        arrow->endItem()->removeArrow(arrow);
        scene()->removeItem(arrow);
        delete arrow;
        arrow = nullptr;
    }
}

void ModelItem::addArrow(Arrow *arrow)
{
    myArrows.append(arrow);
}

QPixmap ModelItem::image() const
{
    QPixmap pixmap(250, 250);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setPen(QPen(Qt::black, 8));
    painter.translate(125, 125);
    painter.drawPolyline(myPolygon);

    return pixmap;
}

bool ModelItem::closeByHotStop(const QPointF &pt) const
{
    QPointF pos = mapFromScene(pt);
    foreach(QPointF hotSpot, hotSpots)
        if(abs(pos.x() - hotSpot.x()) < 8 &&
           abs(pos.y() - hotSpot.y()) < 8)
            return true;
    return false;
}

QPointF ModelItem::closestHotSpot(QPointF pos) const
{
    QPointF closest;
    qreal minDistance = -1;
    foreach(QPointF hs, hotSpots)
    {
        hs = mapToScene(hs);
        qreal curDistance = distance(pos, hs);
        if(curDistance < minDistance || minDistance==-1)
        {
            minDistance = curDistance;
            closest = hs;
        }
    }

    return closest;
}

QVariant ModelItem::itemChange(GraphicsItemChange change,
                     const QVariant &value)
{
    if (change == QGraphicsItem::ItemPositionChange) {
        foreach (Arrow *arrow, myArrows) {
            arrow->updatePosition();
        }
        bModified = true;
    }

    return value;
}
