#ifndef VERTEX_H
#define VERTEX_H

#include <QPoint>
#include <QVector>
#include <QColor>
#include <string>
#include <QRect>
#include <QSize>

class Vertex
{
private:
    int id;
    int rad;
    int flow_sum;

    QColor color;
    QRect rect;


public:
    Vertex(QPoint newPoint);
    Vertex(Vertex* toCopy);
    Vertex(){}

    //set functions
    void setId(int newId);
    void setRad(int newRad);
    void setFlowSum(int newFlowSum);
    void setPoint(QPoint newPoint);
    void setColor(QColor newColor);
    void setRectSize(QSize newRectSize);
    void setRect(QRect newRect);

    //get functions
    int getRad();
    int getFlowSum();
    int getId();
    QPoint getPoint();
    QColor getColor();
    QRect getRect();
    QPoint getCenterPoint();

    //atualize function
    void updateVertexPosition(QPoint newPoint);
    void updateVertexInformations(int newRad, QColor newColor);


    //Sobrecarga de Operadores
    bool operator ==(Vertex *right);
    bool operator !=(Vertex *right);
    void operator  =(Vertex *right);

    friend std::ostream& operator <<(std::ostream& os, const Vertex& right);
    friend std::istream& operator >>(std::istream& is,       Vertex& right);


};

#endif // VERTEX_H
