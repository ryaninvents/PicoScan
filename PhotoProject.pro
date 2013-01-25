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
    geom/chessgen.cpp \
    hardware/opticaldevice.cpp \
    hardware/camera/camera.cpp \
    hardware/camera/opencvcamera.cpp

HEADERS  += mainwindow.h \
    geom/chessgen.h \
    hardware/opticaldevice.h \
    hardware/camera/camera.h \
    hardware/camera/opencvcamera.h

FORMS    += mainwindow.ui


LIBS +=`pkg-config opencv --cflags --libs`
