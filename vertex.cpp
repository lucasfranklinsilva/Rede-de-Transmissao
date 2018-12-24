#include "vertex.h"
#include <fstream>

/********************************************************************************************************/
//Constructor and Destructor
/********************************************************************************************************/
Vertex::Vertex(QPoint newPoint){

    flow_sum = 0;
    rect.setRect(newPoint.x(), newPoint.y(), 50, 50);
    id = 0;
    color.setRgb(255,255,255); //white color as default
}

/*The copy construtor*/
Vertex::Vertex(Vertex* toCopy){
    *this = *toCopy;
}

/********************************************************************************************************/
//set functions
/********************************************************************************************************/
void Vertex::setRad(int newRad){
    rad = newRad;
}

void Vertex::setFlowSum(int newFlowSum){
    flow_sum = newFlowSum;
}

void Vertex::setId(int newId)
{
    id = newId;
}

void Vertex::setPoint(QPoint newPoint){
    rect.moveTo(newPoint.x(), newPoint.y());
}

void Vertex::setColor(QColor newColor){
    color = newColor;
}

void Vertex::setRectSize(QSize newRectSize){
    rect.setSize(newRectSize);
}

void Vertex::setRect(QRect newRect){
    rect = newRect;
}

/********************************************************************************************************/
//get functions
/********************************************************************************************************/
int Vertex::getRad(){
    return rad;
}

int Vertex::getFlowSum(){
    return flow_sum;
}

int Vertex::getId()
{
    return id;
}

QPoint Vertex::getPoint(){
    QPoint rectPoint(rect.x(), rect.y());
    return rectPoint;
}

QColor Vertex::getColor(){
    return color;
}

QRect Vertex::getRect()
{
    return rect;
}

QPoint Vertex::getCenterPoint()
{
    return rect.center();
}

/********************************************************************************************************/
//update functions
/********************************************************************************************************/
void Vertex::updateVertexPosition(QPoint newPoint){
    setPoint(newPoint);
}

void Vertex::updateVertexInformations(int newRad, QColor newColor){
    if (newRad) rad = newRad;
    color = newColor;
}
/********************************************************************************************************/
//Sobrecarga de Operadores
/********************************************************************************************************/
bool Vertex::operator ==(Vertex *right){
    if ( (id == right->id) && (rad == right->rad) && (flow_sum == right->flow_sum) ){
        if ( (rect == right->rect) && (color == right->color) )
            return true;
    }
    return false;
}

bool Vertex::operator !=(Vertex *right){
    if ( (id != right->id) && (rad != right->rad) && (flow_sum != right->flow_sum) ){
        if ( (rect == right->rect) && (color != right->color) )
            return true;
    }
    return false;
}

void Vertex::operator =(Vertex *right){
    id          = right->id;
    rad         = right->rad;
    flow_sum    = right->flow_sum;
    rect        = right->rect;
    color       = right->color;
}

std::ostream& operator <<(std::ostream& os, const Vertex& right){
    os <<                                                                                                    "[  ";
    os << right.id                                                                                        << " , ";
    os << right.flow_sum                                                                                  << " , ";
    os << "( " << right.rect.x() << " , " << right.rect.y() << ")"                                         << ",";
    os << "( "  << right.color.red() << " , " << right.color.green() << " , " << right.color.blue() << ")"  << ",";
    os <<                                                                                                     "]";

    return os;
}
std::istream& operator >>(std::istream& is, Vertex& right){
    char marks[3]; //just to get the marks writen by the ostream
    int x,y;
    int r,g,b;

    is >>                                            marks;
    is >> right.id                                >> marks;
    is >> right.flow_sum                          >> marks;

    is >> marks >> x;
    is >> marks >> y;
    right.setPoint(QPoint(x,y));

    is >> marks;
    is >> r >> marks;
    is >> g >> marks;
    is >> b;

    right.setColor(QColor(r,g,b,255));

    return is;
}
