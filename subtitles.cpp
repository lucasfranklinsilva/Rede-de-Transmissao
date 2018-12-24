#include "subtitles.h"

Subtitles::Subtitles()
{
    minWidthEdge = 3;
    maxWidthEdge = 10;
}

void Subtitles::setConsumer(QPointF points, QColor color)
{
    consumer = QRect(points.x(),points.y()+15,50,50);
    consumerColor = color;
}

void Subtitles::setGenerator(QPointF points, QColor color)
{
    generator = QRect(-60,points.y()+15,50,50);
    generatorColor = color;

}

void Subtitles::setMaxWidthEdge(int newMaxWidthEdge)
{
    maxWidthEdge = (newMaxWidthEdge > 0) ? newMaxWidthEdge : 10;
}

QPolygon Subtitles::calcPolygonPoints(QPoint a, QPoint b){

    QPoint p1, n;
    QPoint p2, p3, p4, p5;
    QVector<QPoint> polygonPoints;

    p1.setX(b.x()-a.x());
    p1.setY(b.y()-a.y());

    n.setX(-p1.y());
    n.setY(p1.x());

    float d = pow((n.x()),2)+pow((n.y()),2);
    d = qSqrt(d);

    float versor[2];
    versor[0] = n.x()/d;
    versor[1] = n.y()/d;

    p2.setX(a.x()-versor[0]*minWidthEdge);
    p2.setY(a.y()-versor[1]*minWidthEdge);

    p3.setX(a.x()+versor[0]*((minWidthEdge+1)));
    p3.setY(a.y()+versor[1]*((minWidthEdge+1)));

    p4.setX(b.x()+versor[0]*((((maxWidthEdge>30) ? 30 : maxWidthEdge)-1)));
    p4.setY(b.y()+versor[1]*((((maxWidthEdge>30) ? 30 : maxWidthEdge)-1)));

    p5.setX(b.x()-versor[0]*((maxWidthEdge>30) ? 30 : maxWidthEdge));
    p5.setY(b.y()-versor[1]*((maxWidthEdge>30) ? 30 : maxWidthEdge));

    polygonPoints.push_back(p5);
    polygonPoints.push_back(p4);
    polygonPoints.push_back(p3);
    polygonPoints.push_back(p2);

    QPolygon pol(polygonPoints);
    return pol;
}

void Subtitles::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QFont font("Times", 10, QFont::Bold);
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setBrush(consumerColor);
    painter->drawEllipse(consumer);

    painter->setBrush(generatorColor);
    painter->drawEllipse(generator);

    painter->setBrush(Qt::white);
    painter->drawPolygon(calcPolygonPoints(QPoint(-185,60), QPoint(-25,60)));
    painter->setFont(font);
    painter->drawText(QPoint(-195,68),"0");
    painter->drawText(QPoint(-23,68),QString::number(maxWidthEdge));
}

QRectF Subtitles::boundingRect() const{ return QRectF(); }
