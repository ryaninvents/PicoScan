#-------------------------------------------------
#
# Project created by QtCreator 2012-11-13T20:24:39
#
#-------------------------------------------------

QT       += core gui

TARGET = PhotoProject
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    geom/chessgen.cpp

HEADERS  += mainwindow.h \
    geom/chessgen.h

FORMS    += mainwindow.ui


LIBS +=`pkg-config opencv --cflags --libs`
