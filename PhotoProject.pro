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
    hardware/camera/opencvcamera.cpp \
    hardware/standards/calibrationstandard.cpp \
    hardware/standards/chessboardstandard.cpp \
    gui/modelviewwidget.cpp \
    gui/aboutdialog.cpp \
    gui/camerasettingsdialog.cpp

HEADERS  += mainwindow.h \
    geom/chessgen.h \
    hardware/opticaldevice.h \
    hardware/camera/camera.h \
    hardware/camera/opencvcamera.h \
    hardware/standards/calibrationstandard.h \
    hardware/standards/chessboardstandard.h \
    gui/modelviewwidget.h \
    gui/aboutdialog.h \
    gui/camerasettingsdialog.h

FORMS    += mainwindow.ui \
    gui/aboutdialog.ui \
    gui/camerasettingsdialog.ui


LIBS += `pkg-config opencv --cflags --libs`

OTHER_FILES += \
    Doxyfile

RESOURCES += \
    icons.qrc


QT += opengl

