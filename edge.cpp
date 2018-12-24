#include "edge.h"

/********************************************************************************************************/
//Constructor and Destructor*/
/********************************************************************************************************/
Edge::Edge(int newVertex1Id, QPoint coordVertex1, int newVertex2Id, QPoint coordVertex2){

    QVector<QPoint> polygonNewPoints ;

    flow = 0;
    upper_limit_flow = 10;
    flowDirection = 1;
    color.setRgb(255,255,255); //white color as default

    vertex1_id = newVertex1Id;
    vertex2_id = newVertex2Id;
    edgeWidth = 10;

    polygonNewPoints = calcPolygonPoints(coordVertex1, coordVertex2);
    QPolygon AUX(polygonNewPoints);
    edgenode = AUX;

    polygonNewPoints = calcDirectionPoints(coordVertex1, coordVertex2);
    QPolygon AUX2(polygonNewPoints);
    edgeDirection = AUX2;

}

/********************************************************************************************************/
//set functions
/********************************************************************************************************/
void Edge::setFlow(float newFlow){
    flow = newFlow;
}

void Edge::setUpperLimitFlow(float newUpperLimitFlow){
    upper_limit_flow = newUpperLimitFlow;
}

void Edge::setVertex1Id(int newVertex1Id){
    vertex1_id = newVertex1Id;
}

void Edge::setVertex2Id(int newVertex2Id){
    vertex2_id = newVertex2Id;
}

void Edge::setEdgeWidth(int newEdgeWidth){
    edgeWidth = newEdgeWidth;
}

void Edge::setVertexId(int newVertex1Id, int newVertex2Id){
    vertex1_id = newVertex1Id;
    vertex2_id = newVertex2Id;
}

void Edge::setColor(QColor newColor){
    color = newColor;
}

void Edge::setEdgenode(QPolygon newEdgeNode){
    edgenode = newEdgeNode;
}

void Edge::setFlowDirection(bool direction){
    flowDirection = direction;
}

/********************************************************************************************************/
//get functions
/********************************************************************************************************/
float Edge::getFlow(){
    return flow;
}

float Edge::getUpperLimitFlow(){
    return upper_limit_flow;
}

int Edge::getVertex1Id(){
    return vertex1_id;
}

int Edge::getVertex2Id(){
    return vertex2_id;
}

int Edge::getEdgeWidth(){
    return edgeWidth;
}

QColor Edge::getColor(){
    return color;
}

QPolygon Edge::getPolygon(){
    return edgenode;
}

QPolygon Edge::getPolygonDirection(){
    return edgeDirection;
}
bool Edge::getFlowDirection(){
    return flowDirection;
}

QLine Edge::getEdgeDirectionAux(){
    return edgeDirectionAux;
}

/********************************************************************************************************/
//Update functions
/********************************************************************************************************/

void Edge::updateEdge(QPoint coordVertex1, QPoint coordVertex2){

    QVector<QPoint> polygonNewPoints;

    edgenode.clear();

    polygonNewPoints = calcPolygonPoints(coordVertex1, coordVertex2);

    QPolygon AUX(polygonNewPoints);
    edgenode = AUX;

    if(flowDirection) polygonNewPoints = calcDirectionPoints(coordVertex1, coordVertex2);
    else polygonNewPoints = calcDirectionPoints(coordVertex2, coordVertex1);
    QPolygon AUX2(polygonNewPoints);
    edgeDirection = AUX2;
}

void Edge::updateFlowDiretion(QPoint coordVertex1, QPoint coordVertex2){

    edgeDirection.clear();

    QVector<QPoint> polygonNewPoints = calcDirectionPoints(coordVertex1, coordVertex2);
    QPolygon AUX2(polygonNewPoints);
    edgeDirection = AUX2;
}


void Edge::upgradeEdgeFlow(int newFlow){
    setFlow(newFlow);
}

//*************************************************************
//  Vetorial Calc to Polygon
//*************************************************************
QVector<QPoint> Edge::calcPolygonPoints(QPoint a, QPoint b){

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

    p2.setX(a.x()+versor[0]*edgeWidth);
    p2.setY(a.y()+versor[1]*edgeWidth);

    p3.setX(a.x()-versor[0]*edgeWidth);
    p3.setY(a.y()-versor[1]*edgeWidth);

    p4.setX(b.x()-versor[0]*edgeWidth);
    p4.setY(b.y()-versor[1]*edgeWidth);

    p5.setX(b.x()+versor[0]*edgeWidth);
    p5.setY(b.y()+versor[1]*edgeWidth);

    polygonPoints.push_back(p2);
    polygonPoints.push_back(p3);
    polygonPoints.push_back(p4);
    polygonPoints.push_back(p5);

    return polygonPoints;
}


QVector<QPoint> Edge::calcDirectionPoints(QPoint a, QPoint b){

    QPoint p1, n, p2, p3;
    QPoint p6, p7, p8;
    QVector<QPoint> directionPoints;

    p1 = QPoint((b.x()-a.x()), (b.y()-a.y()));
    n = QPoint(-p1.y(), p1.x());

    float d = pow((n.x()),2)+pow((n.y()),2);
    d = qSqrt(d);

    float versor[2];
    versor[0] = n.x()/d;
    versor[1] = n.y()/d;

    p2.setX(a.x()+versor[0]*edgeWidth);
    p2.setY(a.y()+versor[1]*edgeWidth);

    p3.setX(a.x()-versor[0]*edgeWidth);
    p3.setY(a.y()-versor[1]*edgeWidth);

    //-----------Calc Direction-----------

    QPoint vecP = QPoint(b.x() - a.x(), b.y() - a.y());
    d = sqrt(pow((vecP.x()),2) + pow((vecP.y()),2));
    versor[0] = vecP.x()/d;
    versor[1] = vecP.y()/d;

    p6 = QPoint(p2.x() + ceil(versor[0]*(d/2.3)),p2.y() + ceil(versor[1]*(d/2.3)));
    p7 = QPoint(a.x() + ceil(versor[0]*(d/2)) , a.y() + ceil(versor[1]*(d/2)));
    p8 = QPoint(p3.x() + ceil(versor[0]*(d/2.3)),p3.y() + ceil(versor[1]*(d/2.3)));

    directionPoints.push_back(p6);
    directionPoints.push_back(p7);
    directionPoints.push_back(p8);

    edgeDirectionAux.setPoints(a, p7);

    return directionPoints;

}

/********************************************************************************************************/
//Sobrecarga de Operadores
/********************************************************************************************************/
bool Edge::operator ==(Edge *right){
    if ((flow == right->flow) && (upper_limit_flow == right->upper_limit_flow)){
        if ((color == right->color) && (edgenode == right->edgenode)){
            if ((vertex1_id == right->vertex1_id) && (vertex2_id == right->vertex2_id)){
                if (flowDirection == right->flowDirection){
                    return true;
                }
            }
        }
    }
    return false;
}

bool Edge::operator !=(Edge *right){
    if ((flow != right->flow) && (upper_limit_flow != right->upper_limit_flow)){
        if ((color != right->color) && (edgenode != right->edgenode)){
            if ((vertex1_id != right->vertex1_id) && (vertex2_id != right->vertex2_id)){
                if (flowDirection != right->flowDirection){
                    return true;

                }
            }
        }
    }
    return false;
}

void Edge::operator  =(Edge *right){
    flow                  = right->flow;
    upper_limit_flow      = right->upper_limit_flow;
    vertex1_id         = right->vertex1_id;
    vertex2_id         = right->vertex2_id;
    flowDirection         = right->flowDirection;
    color                 = right->color;
    edgenode              = right->edgenode;
}

std::ostream& operator <<(std::ostream& os, const Edge& right){
    os <<                                  "[  ";
    os << right.flow                    << " , ";
    os << right.upper_limit_flow        << ",";
    os << "( " << right.color.red()     << " , ";
    os << right.color.green()           << " , ";
    os << right.color.blue() << " )"    << ", ";

    os << right.flowDirection           << " , ";
    os << right.edgeWidth               << " , ";
    os << right.vertex1_id           << " , ";
    os << right.vertex2_id           << "  ]" << std::endl;

    return os;
}

std::istream& operator >>(std::istream& is, Edge& right){
    char marks[3];
    int r,g,b;

    is >>                                            marks;
    is >> right.flow                              >> marks;
    is >> right.upper_limit_flow                  >> marks;

    is >> r >> marks >> g >> marks >> b >> marks;
    right.setColor(QColor(r,g,b,255));

    is >> right.flowDirection                     >> marks;
    is >> right.edgeWidth                         >> marks;
    is >> right.vertex1_id                     >> marks;
    is >> right.vertex2_id;

    return is;
}
