#ifndef EDGE_H
#define EDGE_H

#include <QPolygon>
#include <QtMath>
#include <QLine>
#include "vertex.h"

/*Aresta*/
class Edge
{
private:
    float flow;
    float upper_limit_flow;
    int vertex1_id;
    int vertex2_id;
    int edgeWidth;
    bool flowDirection;


    QColor color;
    QPolygon edgenode;
    QPolygon edgeDirection;
    QLine edgeDirectionAux;

public:
    Edge(int newVertex1Id, QPoint coordVertex1, int newVertex2Id, QPoint coordVertex2);
    Edge(){}

    //set functions
    void setFlow(float newFlow);
    void setUpperLimitFlow(float newUpperLimitFlow);
    void setVertex1Id(int newVertex1Id);
    void setVertex2Id(int newVertex2Id);
    void setEdgeWidth(int newEdgeWidth);
    void setVertexId(int newVertex1Id, int newVertex2Id);
    void setColor(QColor newColor);
    void setEdgenode(QPolygon newEdgeNode);
    void setFlowDirection(bool direction);

    //get functions
    float getFlow();
    float getUpperLimitFlow();
    int getVertex1Id();
    int getVertex2Id();
    int getEdgeWidth();
    bool getFlowDirection();
    QColor getColor();
    QPolygon getPolygon();
    QPolygon getPolygonDirection();
    QLine getEdgeDirectionAux();

    //Calc functions
    QVector<QPoint> calcPolygonPoints(QPoint a, QPoint b);
    QVector<QPoint> calcDirectionPoints(QPoint a, QPoint b);

    //atualize function
    void upgradeEdgeFlow(int newFlow);
    void updateEdge(QPoint coordVertex1, QPoint coordVertex2);
    void updateFlowDiretion(QPoint coordVertex1, QPoint coordVertex2);

    //Sobrecarga de Operadores
    bool operator ==(Edge *right);
    bool operator !=(Edge *right);
    void operator  =(Edge *right);

    friend std::ostream& operator <<(std::ostream& os, const Edge& right);
    friend std::istream& operator >>(std::istream& is,       Edge& right);
};

#endif // EDGE_H




