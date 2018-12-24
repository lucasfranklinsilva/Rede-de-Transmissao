#ifndef SUBTITLES_H
#define SUBTITLES_H

#include <QGraphicsItem>
#include <QPainter>
#include <qmath.h>
#include <QFont>

class Subtitles : public QGraphicsItem
{
private:
    QRect generator, consumer;
    QColor generatorColor, consumerColor;
    int minWidthEdge, maxWidthEdge;

public:
    Subtitles();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;
    void setConsumer(QPointF points, QColor color);
    void setGenerator(QPointF points, QColor color);

    QPolygon calcPolygonPoints(QPoint a, QPoint b);
    void setMaxWidthEdge(int newMaxWidthEdge);
signals:

public slots:

};

#endif // SUBTITLES_H
