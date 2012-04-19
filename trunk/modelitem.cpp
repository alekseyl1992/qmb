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

ModelItem::ModelItem(ItemType itemType, int itemId, QMenu *contextMenu,
             QGraphicsItem *parent, QGraphicsScene *scene)
    : QGraphicsPolygonItem(parent, scene), ItemShadow(this)
{
    myItemType = itemType;
    myContextMenu = contextMenu;
    myId = itemId;
    QPen iPen = pen();
    iPen.setWidth(2);
    setPen(iPen);

    QPainterPath path;
    switch (myItemType) {
        case ItemType::Generator:
            myPolygon << QPointF(-100, -50) << QPointF(100, -50)
                      << QPointF(150, 0) << QPointF(100, 50)
                      << QPointF(-100, 50);
            break;
        case ItemType::Queue:
            myPolygon << QPointF(-100, -50) << QPointF(100, -50)
                      << QPointF(100, 50) << QPointF(-100, 50)
                      << QPointF(-100, -50);
            break;
        case ItemType::Handler:
            path.moveTo(100, 0);
            path.arcTo(50, -50, 50, 50, 0, 90);
            path.arcTo(-50, -50, 50, 50, 90, 90);
            path.arcTo(-50, 0, 50, 50, 180, 90);
            path.arcTo(50, 0, 50, 50, 270, 90);
            path.lineTo(100, -25);
            myPolygon = path.toFillPolygon();
            break;

        case ItemType::Terminator:
            myPolygon << QPointF(-150, 0) << QPointF(-100, -50) << QPointF(100, -50)
                      << QPointF(100, 50)
                      << QPointF(-100, 50);
            break;
    }
    //уменьшаем
    for(QPointF &pnt : myPolygon)
        pnt /= 2;

    setPolygon(myPolygon);

    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
}

ModelItem::~ModelItem()
{

}

void ModelItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    // fill part of polygon
    //устанавливаем градиентную заливку
    QLinearGradient linearGrad(QPointF(0, 0),
                               QPointF(0, boundingRect().height()));
    linearGrad.setColorAt(0, QColor(250, 250, 230));
    linearGrad.setColorAt(1, QColor(210, 210, 190));
    painter->setBrush(QBrush(linearGrad));
    QRectF rect = boundingRect();
    //rect.setTop(rect.bottom() - fract * rect.height());
    painter->drawPolygon(polygon().intersected(rect), fillRule());
    //draw polygon outline
    painter->setPen(pen());
    painter->setBrush(Qt::NoBrush);
    painter->drawPolygon(polygon(), fillRule());
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

void ModelItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    scene()->clearSelection();
    setSelected(true);
    myContextMenu->exec(event->screenPos());
}

QVariant ModelItem::itemChange(GraphicsItemChange change,
                     const QVariant &value)
{
    if (change == QGraphicsItem::ItemPositionChange) {
        foreach (Arrow *arrow, myArrows) {
            arrow->updatePosition();
        }
    }

    return value;
}
