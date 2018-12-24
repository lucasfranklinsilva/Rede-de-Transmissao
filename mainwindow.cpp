#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    option = NULL_;
    auxIndex1 = -1;
    auxIndex2 = -1;
    pressed = false;
    m_x = 0;
    m_y = 0;
    drawingEdge = false;

    //ContextMenu
    this->setContextMenuPolicy(Qt::CustomContextMenu);

    //GraphScene to Subtitles
    scene = new QGraphicsScene(this);

    ui->graphicsView->setScene(scene);

    mySub.setConsumer(ui->graphicsView->mapToScene(ui->graphicsView->geometry().topLeft()),
                       QColor(QColor(255,127,0)));
    mySub.setGenerator(ui->graphicsView->mapToScene(ui->graphicsView->geometry().topRight()),
                       QColor(QColor(28,134,238)));

    scene->addItem(&mySub);
    ui->groupBox->hide();
}

/********************************************************************************************************/
//Events
/********************************************************************************************************/

void MainWindow::paintEvent(QPaintEvent *e)
{

    //qDebug()<<"choram as rosas... pq nao querem estar em pac";
    QPainter *painter = new QPainter(this);
    painter->setRenderHint(QPainter::Antialiasing);
    e->accept();
    painter->setPen(Qt::darkGreen);

    for(unsigned int i = 0; i< g.getNumEdge(); i++){

        QBrush vertexColor(g.getEdge(i)->getColor());
        painter->setBrush(vertexColor);

        painter->drawPolygon(g.getEdge(i)->getPolygon());
        painter->setBrush(Qt::black);
        painter->drawLine(g.getEdge(i)->getEdgeDirectionAux());
        painter->drawPolygon(g.getEdge(i)->getPolygonDirection());
    }

    if(pressed){
        for(unsigned int i = 0; i<(unsigned int)edgesTemp.size(); i++){

            g.atualizeEdge(edgesTemp[i]);
            QBrush vertexColor(g.getEdge(i)->getColor());
            painter->setBrush(vertexColor);
            painter->drawPolygon(g.getEdge(i)->getPolygon());
            painter->setBrush(Qt::black);
            painter->drawLine(g.getEdge(i)->getEdgeDirectionAux());
            painter->drawPolygon(g.getEdge(i)->getPolygonDirection());

        }
    }

    for(unsigned int i = 0; i<g.getNumVertex(); i++){

        if(drawingEdge && (unsigned)auxIndex1==i) {
            QPen framePen(Qt::red);
            framePen.setWidth(6);
            painter->setPen(framePen);
        }
        else{
            QPen framePen(Qt::darkGreen);
            framePen.setWidth(3);
            painter->setPen(framePen);
        }

        g.atualizeVertexColors(i);
        g.atualizeVertexSize(i, g.getMaxFlowVertexSum());


        painter->setBrush(g.getVertex(i)->getColor());

        if(pressed){
            g.atualizeVertex(movVertex,QPoint(m_x, m_y));
            painter->drawEllipse(g.getVertex(i)->getRect());
        }
        painter->drawEllipse(g.getVertex(i)->getRect());
    }

    painter->end();

    mySub.setMaxWidthEdge(g.getUpperLimitFlowGlobal());

    QPoint lastPoint = this->centralWidget()->geometry().bottomRight();
    ui->groupBox->move(lastPoint.x()-290, lastPoint.y()-410);
    //qDebug()<<"rosas sao vermelhas... wololo.. rosas sao azuis";
}

void MainWindow::mouseReleaseEvent(QMouseEvent *e)
{
    if(option == VERTEX && e->button()==Qt::LeftButton){
        addVertex(e->pos());
        repaint();
    }

    else if(option == DELETE && e->button()==Qt::LeftButton){
        deleteV(e);
        deleteE(e);
    }

    pressed = false;
    edgesTemp.clear();
    repaint();
}

void MainWindow::mousePressEvent(QMouseEvent *e){

    if(option == MOVE && e->button()==Qt::LeftButton){
        moveVertex(e);
    }

    else if(option == INFO && e->button()==Qt::LeftButton){
        for(unsigned int i = 0; i<g.getNumEdge(); i++){
            EdgeInfoTooltip(i, e->pos());
        }
        for(unsigned int i = 0; i<g.getNumVertex(); i++){
            VertexInfoTooltip(i, e->pos());
        }
    }

    else if(option==INVERT && e->button()==Qt::LeftButton){
        if(!isVertex(e->pos())) invertDirection(e);
    }

    else if(option == EDGE && e->button()==Qt::LeftButton){
        createEdge(e);
    }

    if(e->button()==Qt::RightButton){
        for(unsigned int i = 0; i<g.getNumEdge(); i++){
            if(g.getEdge(i)->getPolygon().containsPoint(e->pos(),Qt::WindingFill)){
                auxIndex1 = i;
                contextMenu(e->pos());
                repaint();
            }
        }
        auxIndex1 = -1;
    }
    repaint();
}

void MainWindow::mouseMoveEvent(QMouseEvent *e){

    if ((pressed) && e->DragMove)
    {
        if(e->pos().x() < this->width() && e->pos().y() < this->height()
                && e->pos().x() > ui->mainToolBar->width() && e->pos().y() > ui->toolBar->height()*2){
        int delta_x = e->x() - m_previous_mouse_x;      //Calcula a posicao X do Drag
        int delta_y = e->y() - m_previous_mouse_y;      //Calcula a posicao Y do Drag

        m_x += delta_x;
        m_y += delta_y;

        m_previous_mouse_x = e->x();
        m_previous_mouse_y = e->y();
        }
        repaint();
    }

    repaint();

}
/********************************************************************************************************/
// Operations to drawing
/********************************************************************************************************/

void MainWindow::createEdge(QMouseEvent *e){

    for(unsigned int i = 0; i<g.getNumVertex(); i++){

        float d = normaVertex(i, e->pos());

        if(g.getVertex(i)->getRect().contains(e->pos()) && d <(int)g.getVertex(i)->getRect().size().width()/2){
            addEdge(i);
        }
    }
}

void MainWindow::moveVertex(QMouseEvent *e){

    for(unsigned int i = 0; i<g.getNumVertex(); i++){

        float d = normaVertex(i, e->pos());

        if(g.getVertex(i)->getRect().contains(e->pos()) && d <(int)g.getVertex(i)->getRect().size().width()/2){
            pressed = true;
            m_x = g.getVertex(i)->getPoint().x();
            m_y = g.getVertex(i)->getPoint().y();
            m_previous_mouse_x = e->x();
            m_previous_mouse_y = e->y();
            movVertex = i;
            edgesTemp = g.getEdgeIndexbyVertex(g.getVertex(i)->getId());
        }
    }
}

void MainWindow::invertDirection(QMouseEvent *e){

    for(int i = g.getNumEdge()-1; i>=0; i--){
        if(g.getEdge(i)->getPolygon().containsPoint(e->pos(),Qt::WindingFill)){
            bool direction = (g.getFlowDirection(i)) ? 0 : 1;
            g.atualizeFlowDirection(direction,i);
            repaint();
            break;
        }
    }

}

void MainWindow::deleteV(QMouseEvent *e){

    if(g.getNumVertex()>0){
        int numVertex = g.getNumVertex()-1;

        for(int i = numVertex; i>=0; i--){

            float d = normaVertex(i, e->pos());

            if(g.getVertex(i)->getRect().contains(e->pos()) && d <(int)g.getVertex(i)->getRect().size().width()/2){
                int id = g.getVertex(i)->getId();
                edgesTemp = g.deleteVertex(i);

                unsigned int tam = edgesTemp.size();

                for(unsigned int j = 0; j<tam; j++)
                {
                    g.deleteEdge(edgesTemp[0]);
                    edgesTemp = g.getEdgeIndexbyVertex(id);
                }
                edgesTemp.clear();
                break;
            }
        }
    }
    repaint();

}

void MainWindow::deleteE(QMouseEvent *e){

    if(g.getNumVertex()>0){
        int numEdge = g.getNumEdge()-1;
        for(int i = numEdge; i>=0; i--){
            if(g.getEdge(i)->getPolygon().containsPoint(e->pos(),Qt::WindingFill)){
                g.deleteEdge(i);
                break;
            }
        }
    }
    repaint();

}
/********************************************************************************************************/
//General Functions
/********************************************************************************************************/

float MainWindow::normaVertex(int index, QPoint e){

    QPoint center = g.getVertex(index)->getCenterPoint();
    float d = pow((center.x()-e.x()),2)+pow((center.y()-e.y()),2);
    return qSqrt(d);

}

bool MainWindow::isVertex(QPoint p){
    for(unsigned int i = 0; i<g.getNumVertex(); i++){

        float d = normaVertex(i, p);

        if(g.getVertex(i)->getRect().contains(p) && d <(int)g.getVertex(i)->getRect().size().width()/2){
            return true;
        }
    }

    return false;
}

void MainWindow::addVertex(QPoint point){

    Vertex *newVertex = new Vertex(point);
    g.newVertex(newVertex);

}

void MainWindow::addEdge(int index)
{

    if(auxIndex1 == -1){ auxIndex1 = index;     drawingEdge = true; }

    else if(auxIndex1!=index){ auxIndex2 = index; }

    if(auxIndex1 >= 0 && auxIndex2 >= 0){
        if(!g.hasEdge(g.getVertex(auxIndex1)->getId(), g.getVertex(auxIndex2)->getId())){
        drawingEdge = false;
        Edge* newEdge = new Edge(g.getVertex(auxIndex1)->getId(), g.getVertexCenterPosition(auxIndex1), g.getVertex(auxIndex2)->getId(), g.getVertexCenterPosition(auxIndex2));
        g.newEdge(newEdge);
        auxIndex1 = auxIndex2 = -1;
        repaint();

    }
        else { auxIndex2 = -1; }
    }
}

void MainWindow::VertexInfoTooltip(int index, QPoint position){

    QRect rect(position.x(), position.y(), 50,50);
    QFont serifFont("Times", 15, QFont::Bold);
    QToolTip::setFont(serifFont);
    QString toolTipText = (g.getVertex(index)->getFlowSum()>0) ?
            "<nobr><font color=\"red\">Generating: </font></nobr> " + QString::number(g.getVertex(index)->getFlowSum()):
                "<nobr><font color=\"red\">Consuming: </font></nobr> " + QString::number(g.getVertex(index)->getFlowSum()*(-1));

    if(g.getVertex(index)->getFlowSum()==0) toolTipText = "<nobr><font color=\"red\">Neutral</font></nobr>";

    float d = normaVertex(index, position);

    if(g.getVertex(index)->getRect().contains(position) && d <(int)g.getVertex(index)->getRect().size().width()/2){
        QToolTip::showText(this->mapToGlobal(position), toolTipText, this,rect);
    }  else {
        QToolTip::hideText();
    }
}

void MainWindow::EdgeInfoTooltip(int index, QPoint position){

    QRect rect(position.x(), position.y(), 50,50);
    QFont serifFont("Times", 15, QFont::Bold);
    QToolTip::setFont(serifFont);
    QString toolTipText =
            "<nobr><font color=\"red\">Flow: </font></nobr> " + QString::number(g.getEdge(index)->getFlow()) +
            "<br/><nobr><font color=\"red\">Max Flow: </font></nobr>" + QString::number(g.getEdge(index)->getUpperLimitFlow());

    if(g.getEdge(index)->getPolygon().containsPoint(position,Qt::WindingFill)){
        QToolTip::showText(this->mapToGlobal(position), toolTipText, this,rect);
    }
    else {
        QToolTip::hideText();
    }
}

void MainWindow::saveExit(){

    int btn = QMessageBox::question(this, "Save?", "Save Project?", QMessageBox::Yes, QMessageBox::No);

    if(btn==QMessageBox::Yes) {
        if(file.isEmpty()){
            QString temp = QFileDialog::getSaveFileName(this, tr("Open File"), "", tr("Files (*.fpac)"));
            if (temp != ""){
                file = temp;
            }
        }
        g.save(file);
        file.clear();
    }
}

/********************************************************************************************************/
//Menu Actions
/********************************************************************************************************/

void MainWindow::on_action_Vertex_triggered()
{
    option = checkedAction(VERTEX);
    uncheckedAction();
}

void MainWindow::on_action_Edge_triggered()
{
    option = checkedAction(EDGE);
    uncheckedAction();
    if(option != NULL_) QGuiApplication::setOverrideCursor(Qt::PointingHandCursor);
}

void MainWindow::on_action_Delete_triggered()
{
    option = checkedAction(DELETE);
    uncheckedAction();
    if(option != NULL_) QGuiApplication::setOverrideCursor(Qt::ForbiddenCursor);
}

void MainWindow::on_action_Direction_triggered()
{
    option = checkedAction(INVERT);
    uncheckedAction();
    if(option != NULL_) QGuiApplication::setOverrideCursor(Qt::CrossCursor);
}

void MainWindow::on_action_Move_triggered()
{
    option = checkedAction(MOVE);
    uncheckedAction();
    if(option != NULL_) QGuiApplication::setOverrideCursor(Qt::SizeAllCursor);
}

void MainWindow::on_action_Info_triggered()
{
    option = checkedAction(INFO);
    uncheckedAction();
    if(option != NULL_) QGuiApplication::setOverrideCursor(Qt::WhatsThisCursor);
}

void MainWindow::on_action_Subtitles_triggered()
{

    if(ui->action_Subtitles->isChecked()) ui->groupBox->show();
    else ui->groupBox->hide();

}

MainWindow::STATUS MainWindow::checkedAction(STATUS actual){
    return (option != actual) ? actual : NULL_;
}

void MainWindow::uncheckedAction(){
    if(ui->action_Direction->isChecked() && option!=INVERT) ui->action_Direction->setChecked(false);
    if(ui->action_Delete->isChecked() && option!=DELETE) ui->action_Delete->setChecked(false);
    if(ui->action_Edge->isChecked() && option!=EDGE)ui->action_Edge->setChecked(false);
    if(ui->action_Vertex->isChecked() && option!=VERTEX) ui->action_Vertex->setChecked(false);
    if(ui->action_Move->isChecked() && option!=MOVE) ui->action_Move->setChecked(false);
    if(ui->action_Info->isChecked() && option!=INFO) ui->action_Info->setChecked(false);
    QGuiApplication::setOverrideCursor(Qt::ArrowCursor);
    drawingEdge = false;
    auxIndex1=-1;
    repaint();
}

void MainWindow::on_action_Open_triggered()
{
    QString temp = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Files (*.fpac)"));

    if (temp != ""){
        file = temp;
        g.load(file);
    }
}

void MainWindow::on_action_Exit_triggered()
{
    saveExit();
    QApplication::quit();
}

void MainWindow::on_action_Save_As_triggered()
{
    QString temp = QFileDialog::getSaveFileName(this, tr("Open File"), "", tr("Files (*.fpac)"));
    if (temp != ""){
        file = temp;
    }
    g.save(file);
}

void MainWindow::flowDialog_triggered()
{
    bool ok;
    float actualValue = g.getEdge(auxIndex1)->getFlow();
    float max  = g.getEdge(auxIndex1)->getUpperLimitFlow();
    int value = QInputDialog::getDouble(this, tr("Flow"),tr("Value"), actualValue, 0, max, 2, &ok);
    if (ok){
        g.getEdge(auxIndex1)->setFlow(value);
        g.atualizeEdgeColor(value, auxIndex1);
    }
    repaint();
}

void MainWindow::flowCapacityDialog_triggered(){

    bool ok;
    float actualValue = g.getEdge(auxIndex1)->getUpperLimitFlow();
    int value = QInputDialog::getDouble(this, tr("Flow Capacity"),tr("Value"), actualValue, 0, 9999, 2, &ok);
    if (ok){
        g.getEdge(auxIndex1)->setUpperLimitFlow(value);
        g.atualizeEdgeSize(auxIndex1);
        g.atualizeActualFlow(auxIndex1);
        g.atualizeAllEdgesSize();
    }
    repaint();
}

void MainWindow::on_action_Save_triggered()
{
    if(file.isEmpty()){
        QString temp = QFileDialog::getSaveFileName(this, tr("Open File"), "", tr("Files (*.fpac)"));
        if (temp != ""){
            file = temp;
        }
    }
    g.save(file);
}


void MainWindow::contextMenu(QPoint p) {

    QMenu myMenu;
    myMenu.addAction("Set Flow Capacity", this, SLOT(flowCapacityDialog_triggered()));
    myMenu.addAction("Set Flow", this, SLOT(flowDialog_triggered()));
    myMenu.exec(pos()+p);
    repaint();

}

void MainWindow::on_action_New_triggered()
{
    saveExit();
    g.clearAll();
    repaint();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_action_About_triggered()
{
    QMessageBox msgBox;
    msgBox.setText("Projeto Final - Programação Assistida Por Computador\nEngenharia de Sistemas\nAcadêmicos:\nLucas Franklin\nJuliana Miranda\nProfessor Alberto Miranda");
    msgBox.exec();
}
