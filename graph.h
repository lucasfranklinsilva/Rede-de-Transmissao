#ifndef GRAPH_H
#define GRAPH_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QSize>

#include <fstream>
#include <math.h>

#include "edge.h"

class Graph
{
private:

    QVector<Edge*> g;
    QVector<Vertex*> v;
    int auxVertexId;

public:
    Graph();

    //general functions
    double calcPercentual(float actualValue, float totalValue);///changed
    //removi duas funções daqui (min e max) pois não pertencem à graph e são globais mesmo

    //get functions
    bool hasEdge(int index1,int index2);
    bool getFlowDirection(int index);
    int getIndexByID(int id);
    unsigned int getNumVertex();
    unsigned int getNumEdge();
    float getUpperLimitFlowGlobal();///new
    float getMaxFlowVertexSum();
    QVector<int> getEdgeIndexbyVertex(int vertexIndex);
    QPoint getVertexCenterPosition(int index);
    Vertex* getVertex(int index);
    Edge* getEdge(int index);

    //atualize or upgrade functions
    void atualizeVertex(int index, QPoint newCoord);
    void atualizeEdge(int edgeIndex);
    void atualizeFlowDirection(bool direction, int edgeIndex);
    void atualizeActualFlow(int edgeIndex);
    void atualizeEdgeColor(int actualFlow, int edgeIndex);
    void atualizeEdgeSize(int edgeIndex);
    void atualizeVertexSize(int vertexIndex, int maxValue);
    void atualizeAllEdgesSize();
    void atualizeVertexColors(int vertexIndex);
    void upgradeEdgeFlow(int index, int newFlow);
    QColor colorScaleGreenToRed(double x);

    //insert actions
    void newVertex(Vertex* newConnection);
    void newEdge(Vertex* connection1, Vertex* connection2);
    void newEdge(int Vertex1Id, int Vertex2Id);
    void newEdge(Edge* newEdge);

    //delete actions
    QVector<int> deleteVertex(int index);
    void deleteEdge(int index);

    //files actions
    void save(QString arq);
    bool load(QString arq);
    void clearAll();


    void teste(Vertex *tempVertex);

    //Operators actions
    bool operator ==(Graph *right);
    bool operator !=(Graph *right);

    friend std::ostream& operator <<(std::ostream& os, const Graph& right);
    friend std::istream& operator >>(std::istream& is,       Graph& right);

};

#endif // GRAPH_H
