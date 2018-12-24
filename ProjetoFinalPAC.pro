#-------------------------------------------------
#
# Project created by QtCreator 2014-05-17T08:54:16
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ProjetoFinalPAC
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    vertex.cpp \
    edge.cpp \
    graph.cpp \
    subtitles.cpp

HEADERS  += mainwindow.h \
    vertex.h \
    edge.h \
    graph.h \
    subtitles.h

FORMS    += mainwindow.ui

RESOURCES += \
    files.qrc
