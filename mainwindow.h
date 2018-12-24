#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QString>
#include <QDebug>
#include "graph.h"
#include <QPainter>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QInputDialog>
#include <QToolTip>
#include <complex>
#include <QMessageBox>
#include <QScrollArea>
#include <QGraphicsScene>
#include <subtitles.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:

    Ui::MainWindow *ui;
    Graph g;
    enum STATUS{ NULL_, VERTEX, EDGE, MOVE, DELETE, INVERT, INFO};  //Menu States
    STATUS option;
    int auxIndex1, auxIndex2;   //Aux index
    bool pressed;               //Bool pressed mouse button
    int m_previous_mouse_x, m_previous_mouse_y; //Mouse movimentations aux
    int m_x, m_y;     //Mouse movimentation values
    int movVertex;      //Vertex in move
    QVector<int> edgesTemp;     //Endges Aux
    QString file;
    bool drawingEdge;

    //Subtitles
    QGraphicsScene *scene;
    Subtitles mySub;

public:

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void addVertex(QPoint point);
    void addEdge(int index);

    STATUS checkedAction(STATUS actual);
    void uncheckedAction();

    void contextMenu(QPoint p);

    //Events
    void paintEvent(QPaintEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);

    //ToolTips
    void VertexInfoTooltip(int index, QPoint position);
    void EdgeInfoTooltip(int index, QPoint position);

    // Operations to drawing
    void moveVertex(QMouseEvent *e);
    void invertDirection(QMouseEvent *e);
    void createEdge(QMouseEvent *e);
    void deleteV(QMouseEvent *e);
    void deleteE(QMouseEvent *e);

    float normaVertex(int index, QPoint e);
    bool isVertex(QPoint p);
    void saveExit();

    //Menu actions

private slots:
    void on_action_Vertex_triggered();
    void on_action_Edge_triggered();
    void on_action_Delete_triggered();
    void on_action_Direction_triggered();
    void on_action_Open_triggered();
    void on_action_Exit_triggered();
    void on_action_Save_As_triggered();
    void on_action_Move_triggered();
    void on_action_Save_triggered();
    void on_action_Info_triggered();
    void on_action_New_triggered();
    void flowDialog_triggered();
    void flowCapacityDialog_triggered();
    void on_action_Subtitles_triggered();
    void on_action_About_triggered();
};

#endif // MAINWINDOW_H
