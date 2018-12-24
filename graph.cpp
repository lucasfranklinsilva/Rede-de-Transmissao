#include "graph.h"

#include <QDebug>

/********************************************************************************************************/
//Global functions
/********************************************************************************************************/
int min(int a, int b) { return ((a < b) ? a : b); }
int max(int a, int b) { return ((a > b) ? a : b); }


/********************************************************************************************************/
//general functions
/********************************************************************************************************/
/*Compute the actual color give by green to red in RGB color
 *Considerations: green is lower than red;
*/
QColor Graph::colorScaleGreenToRed(double x) {

    QColor newColor;

    newColor.setRed  (min(255,(int)(255*  x   /0.5)));
    newColor.setGreen(min(255,(int)(255*(1-x)/0.5)));
    newColor.setBlue(0);
    return newColor;
}

/*Compute the actual percentualSize of the flow
 *This function only give in consideration the actualFlow and the upperLimitFlow
 *3 rule to compute the percentualSize
 *->this functions is created to serve for calculating the GreenToRed color and the size of the edge
*/
Graph::Graph(){
    auxVertexId = 0;
}

double Graph::calcPercentual(float actualValue, float totalValue){

    if (totalValue == 0) return 0;
    double percentual = double(actualValue / totalValue);
    return percentual;
}

/********************************************************************************************************/
//atualize or upgrade functions
/********************************************************************************************************/
void Graph::atualizeVertexColors(int vertexIndex){

    int sum = 0;

    for(unsigned int i = 0; i<getNumEdge(); i++){
        if(g[i]->getVertex1Id() == v[vertexIndex]->getId()){
            if(g[i]->getFlowDirection()) sum+=g[i]->getFlow();
            else sum-=g[i]->getFlow();
        }
    }

    for(unsigned int i = 0; i<getNumEdge(); i++){
        if(g[i]->getVertex2Id() == v[vertexIndex]->getId()){
            if(g[i]->getFlowDirection()) sum-=g[i]->getFlow();
            else sum+=g[i]->getFlow();
        }
    }

    v[vertexIndex]->setFlowSum(sum);

    if(sum>0) v[vertexIndex]->setColor(QColor(255,127,0));       //Consumidor
    else if(sum<0) v[vertexIndex]->setColor(QColor(28,134,238)); //Gerador
    else v[vertexIndex]->setColor(QColor(255,255,255));

}

void Graph::atualizeVertexSize(int vertexIndex, int maxValue){
    double percUpperFlow = calcPercentual(v[vertexIndex]->getFlowSum() , maxValue);

    if (0 > percUpperFlow){
        percUpperFlow *= -1;
    }
    unsigned int newSize(min (100, ( max(30, percUpperFlow*maxValue ))) );

    QSize newRectSize(newSize, newSize);
    v[vertexIndex]->setRectSize(newRectSize);
    QVector<int> edgesTemp = getEdgeIndexbyVertex(v[vertexIndex]->getId());
    for(unsigned int i = 0; i<(unsigned int)edgesTemp.size(); i++){
        atualizeEdge(edgesTemp[i]);
    }
}

void Graph::atualizeEdgeColor(int actualFlow, int edgeIndex){
    double percColor = calcPercentual(actualFlow, g[edgeIndex]->getUpperLimitFlow() );

    QColor newColor = colorScaleGreenToRed(percColor);
    g[edgeIndex]->setColor(newColor);
}

void Graph::atualizeEdgeSize(int edgeIndex){
    double percUpperFlow = calcPercentual(g[edgeIndex]->getUpperLimitFlow() , getUpperLimitFlowGlobal());
    Edge newEdge = *g[edgeIndex];

    newEdge.setEdgeWidth(min (30, max(5, percUpperFlow*getUpperLimitFlowGlobal() )));

    int id1 = getIndexByID(g[edgeIndex]->getVertex1Id());
    int id2 = getIndexByID(g[edgeIndex]->getVertex2Id());

    newEdge.updateEdge(v[id1]->getCenterPoint(),v[id2]->getCenterPoint());

    *g[edgeIndex] = newEdge;
}

void Graph::atualizeAllEdgesSize(){
    QColor defaultColor(255,255,255);

    for (int i = 0; i < g.size(); i++){
        atualizeEdgeSize(i);

        if (defaultColor != g[i]->getColor()){
            atualizeEdgeColor(g[i]->getFlow(), i);
        }
    }
}

void Graph::atualizeFlowDirection(bool direction, int edgeIndex){

    g[edgeIndex]->setFlowDirection(direction);

    int id1 = getIndexByID(g[edgeIndex]->getVertex1Id());
    int id2 = getIndexByID(g[edgeIndex]->getVertex2Id());

    if(direction==true) g[edgeIndex]->updateFlowDiretion(v[id1]->getCenterPoint(),v[id2]->getCenterPoint());
    else g[edgeIndex]->updateFlowDiretion(v[id2]->getCenterPoint(), v[id1]->getCenterPoint());
}

void Graph::upgradeEdgeFlow(int index, int newFlow){
    g[index]->upgradeEdgeFlow(newFlow);
}


void Graph::atualizeActualFlow(int edgeIndex){
    if(g[edgeIndex]->getFlow() > g[edgeIndex]->getUpperLimitFlow()) g[edgeIndex]->setFlow(g[edgeIndex]->getUpperLimitFlow());
}

/********************************************************************************************************/
//insert actions
/********************************************************************************************************/

void Graph::newVertex(Vertex* newConnection){
    v.push_back(newConnection);
    auxVertexId+=1;

    v[v.size()-1]->setId(auxVertexId);
}

void Graph::newEdge(Vertex* connection1, Vertex* connection2){
    int id1 = connection1->getId();
    int id2 = connection2->getId();

    Edge* newEdge = new Edge (id1,v[id1]->getPoint(),id2,v[id2]->getPoint());

    g.push_back(newEdge);
}

void Graph::newEdge(int Vertex1Id, int Vertex2Id){

    Edge* newEdge = new Edge (Vertex1Id,v[Vertex1Id]->getPoint(),Vertex2Id,v[Vertex2Id]->getPoint());
    g.push_back(newEdge);

}

void Graph::newEdge(Edge* newEdge){
    g.push_back(newEdge);
}

/********************************************************************************************************/
//delete actions
/********************************************************************************************************/
QVector<int> Graph::deleteVertex(int index){
    int Id = v[index]->getId();

    v.remove(index);

    return getEdgeIndexbyVertex(Id);
}

void Graph::deleteEdge(int index){
    g.remove(index);
}

/********************************************************************************************************/
//atualize actions
/********************************************************************************************************/

void Graph::atualizeVertex(int index, QPoint newCoord){
    v[index]->setPoint(newCoord);
}

void Graph::atualizeEdge(int edgeIndex){

    int index1 = getIndexByID(g[edgeIndex]->getVertex1Id());
    int index2 = getIndexByID(g[edgeIndex]->getVertex2Id());

    g[edgeIndex]->updateEdge(v[index1]->getCenterPoint(), v[index2]->getCenterPoint());
}

/********************************************************************************************************/
//files actions
/********************************************************************************************************/

void Graph::save(QString arq){
    int sizeofVertex = v.size();
    int sizeofEdges  = g.size();

   // QString apendix(".fpac");
   // arq.append(apendix);

    std::ofstream ofs(arq.toStdString().c_str(),std::ofstream::out);
    if (!ofs.is_open() ) {
        return;
    }

    ofs << sizeofVertex << std::endl;
    for (int i = 0; i < sizeofVertex; i++){
        ofs << *v[i];
    }


    ofs << std::endl;
    ofs << sizeofEdges << std::endl;
    for (int i = 0; i < sizeofEdges; i++){
        ofs << *g[i];
    }

    ofs.close();
}

void Graph::teste(Vertex *tempVertex){
    Vertex testVertex = *tempVertex;
    newVertex(&testVertex);
}

bool Graph::load(QString arq){
    clearAll();

    char marks[2];
    char mark1;

    std::ifstream ins(arq.toStdString().c_str(),std::ifstream::in);
    if (!ins.is_open() ) {
        return false;
    }

    unsigned int sizeofVertex;
    ins >> sizeofVertex;
    for (unsigned int i = 0; i < sizeofVertex; i++){
        Vertex *tempVertex = new Vertex();
        ins >> *tempVertex;
        newVertex(tempVertex);
    }

    if (sizeofVertex){
        ins >> marks;
    }

    unsigned int sizeofEdges;
    QVector<QPoint> polygonNewPoints;
    ins >> sizeofEdges;
    for (unsigned int i = 0; i < sizeofEdges; i++){
        polygonNewPoints.clear();

        Edge *tempEdge = new Edge();
        ins >> *tempEdge;

        int index1 = getIndexByID(tempEdge->getVertex1Id());
        int index2 = getIndexByID(tempEdge->getVertex2Id());

        polygonNewPoints = tempEdge->calcPolygonPoints(v[index1]->getCenterPoint(), v[index2]->getCenterPoint());
        QPolygon AUX(polygonNewPoints);
        tempEdge->setEdgenode(AUX);
        tempEdge->updateFlowDiretion(v[index1]->getCenterPoint(), v[index2]->getCenterPoint());
        newEdge(tempEdge);

        ins >> mark1;
    }

    ins.close();
    return true;
}

void Graph::clearAll(){
    g.clear();
    v.clear();
    auxVertexId = 0;
}

/********************************************************************************************************/
//Get Functions
/********************************************************************************************************/

unsigned int Graph::getNumVertex()
{
    return v.size();
}

unsigned int Graph::getNumEdge()
{
    return g.size();
}

bool Graph::getFlowDirection(int index){
    return g[index]->getFlowDirection();
}

int Graph::getIndexByID(int id)
{
    for(unsigned int i = 0; i<(unsigned int)v.size(); i++){
        if(v[i]->getId()==id){
            return i;
        }
    }

    return -1;
}

QPoint Graph::getVertexCenterPosition(int index)
{
    return v[index]->getCenterPoint();
}

Vertex* Graph::getVertex(int index){
    return v[index];
}

Edge* Graph::getEdge(int index){
    return g[index];
}

QVector<int> Graph::getEdgeIndexbyVertex(int vertexIndex)
{
    QVector<int> edges;
    for(unsigned int i = 0; i<getNumEdge(); i++){
        if(g[i]->getVertex1Id() == vertexIndex || g[i]->getVertex2Id() == vertexIndex)
            edges.push_back(i);
    }
    return edges;
}

bool Graph::hasEdge(int index1,int index2){
    for(unsigned int i = 0; i<getNumEdge(); i++){
        if((g[i]->getVertex1Id() == index1 && g[i]->getVertex2Id() == index2) ||
                (g[i]->getVertex1Id() == index2 && g[i]->getVertex2Id() == index1))
            return true;
    }
    return false;
}

float Graph::getUpperLimitFlowGlobal(){
    float upper = 0;
    for(int i= 0; i < g.size() ; i++ ){
        if (upper < g[i]->getUpperLimitFlow()){
            upper = g[i]->getUpperLimitFlow();
        }
    }

    return upper;
}

float Graph::getMaxFlowVertexSum(){
    float upper = 0;
    for(int i= 0; i < v.size() ; i++ ){
        if (upper < abs(v[i]->getFlowSum()) ){
            upper = abs(v[i]->getFlowSum());
        }
    }
    return upper;
}

/********************************************************************************************************/
//Operators actions
/********************************************************************************************************/
//QVector<Edge*> g;       //the graph itself
//QVector<Vertex*> v;     //only for those who are not connected
bool Graph::operator ==(Graph *right){
    if(g.size() != right->g.size()) return false;

    int size = g.size();
    for (int i = 0; i < size; i++ ){
        if (g[i] != right->g[i]){
            return false;
        }
    }

    if(v.size() != right->v.size()) return false;

    size = v.size();
    for (int i = 0; i < size; i++ ){
        if (v[i] != right->v[i]){
            return false;
        }
    }

    return true;
}
bool Graph::operator !=(Graph *right){
    if(g.size() == right->g.size()) return false;

    int size = g.size();
    for (int i = 0; i < size; i++ ){
        if (g[i] == right->g[i]){
            return false;
        }
    }

    if(v.size() == right->v.size()) return false;

    size = v.size();
    for (int i = 0; i < size; i++ ){
        if (v[i] == right->v[i]){
            return false;
        }
    }

    return true;
}

std::ostream& operator <<(std::ostream& os, const Graph& right){
    os << "Vertex: " << std::endl;

    int size = right.v.size();
    os << size;
    for (int i = 0; i < size; i++ ){
        os << right.v[i];
    }
    os << std::endl;
    os << "Graph: " << std::endl;

    size = right.g.size();
    os << size;
    for (int i = 0; i < size; i++ ){
        os << right.g[i];
    }

    os << std::endl;

    return os;
}

std::istream& operator >>(std::istream& is, Graph& right){
    std::string marks;

    Edge newEdge;
    Vertex newVertex;

    is >> marks;

    int size;
    is >> size;
    for (int i = 0; i < size; i++ ){
        is >> newVertex;
        right.v.push_back(&newVertex);
    }

    is >> marks;
    is >> marks >> marks;

    is >> size;
    for (int i = 0; i < size; i++ ){
        is >> newEdge;
        right.newEdge(newEdge.getVertex1Id(), newEdge.getVertex2Id());
    }

    is >> marks;
    is >> marks >> marks;

    return is;
}
