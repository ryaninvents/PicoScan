#-------------------------------------------------
#
# Project created by QtCreator 2012-11-13T20:24:39
#
#-------------------------------------------------

QT       += core gui

TARGET = ScanStudio
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    hardware/opticaldevice.cpp \
    hardware/camera/camera.cpp \
    hardware/camera/opencvcamera.cpp \
    hardware/standards/calibrationstandard.cpp \
    hardware/standards/chessboardstandard.cpp \
    gui/modelviewwidget.cpp \
    gui/aboutdialog.cpp \
    gui/camerasettingsdialog.cpp \
    geom/sheet.cpp \
    hardware/projector/projectionscreen.cpp \
    hardware/projector/projectionpattern.cpp \
    gui/imageviewwidget.cpp \
    gui/camerastreamwidget.cpp \
    gui/calibrationstandarddialog.cpp \
    hardware/scanmanager.cpp

HEADERS  += mainwindow.h \
    hardware/opticaldevice.h \
    hardware/camera/camera.h \
    hardware/camera/opencvcamera.h \
    hardware/standards/calibrationstandard.h \
    hardware/standards/chessboardstandard.h \
    gui/modelviewwidget.h \
    gui/aboutdialog.h \
    gui/camerasettingsdialog.h \
    geom/sheet.h \
    hardware/projector/projectionscreen.h \
    hardware/projector/projectionpattern.h \
    gui/imageviewwidget.h \
    gui/camerastreamwidget.h \
    gui/calibrationstandarddialog.h \
    hardware/scanmanager.h

FORMS    += mainwindow.ui \
    gui/aboutdialog.ui \
    gui/camerasettingsdialog.ui \
    gui/calibrationstandarddialog.ui


LIBS += `pkg-config opencv --cflags --libs`

OTHER_FILES += \
    Doxyfile \
    LICENSE \
    gui/icons/oxygen/zoom-select.png \
    gui/icons/oxygen/zoom-previous.png \
    gui/icons/oxygen/zoom-out.png \
    gui/icons/oxygen/zoom-original.png \
    gui/icons/oxygen/zoom-next.png \
    gui/icons/oxygen/zoom-in.png \
    gui/icons/oxygen/zoom-draw.png \
    gui/icons/oxygen/zoom-2-to-1.png \
    gui/icons/oxygen/zoom-1-to-2.png \
    gui/icons/oxygen/window-close.png \
    gui/icons/oxygen/view-statistics.png \
    gui/icons/oxygen/view-restore.png \
    gui/icons/oxygen/view-fullscreen.png \
    gui/icons/oxygen/view-barcode.png \
    gui/icons/oxygen/video-television.png \
    gui/icons/oxygen/video-projector.png \
    gui/icons/oxygen/video-display.png \
    gui/icons/oxygen/transform-move.png \
    gui/icons/oxygen/tools-wizard.png \
    gui/icons/oxygen/task-reject.png \
    gui/icons/oxygen/task-complete.png \
    gui/icons/oxygen/task-attention.png \
    gui/icons/oxygen/task-accepted.png \
    gui/icons/oxygen/system-shutdown.png \
    gui/icons/oxygen/shapes.png \
    gui/icons/oxygen/scanner-big.png \
    gui/icons/oxygen/scanner.png \
    gui/icons/oxygen/roll.png \
    gui/icons/oxygen/process-stop.png \
    gui/icons/oxygen/printer-laser.png \
    gui/icons/oxygen/printer.png \
    gui/icons/oxygen/preferences-system.png \
    gui/icons/oxygen/preferences-desktop-color.png \
    gui/icons/oxygen/office-chart-line.png \
    gui/icons/oxygen/office-chart-area-stacked.png \
    gui/icons/oxygen/media-skip-forward.png \
    gui/icons/oxygen/media-skip-backward.png \
    gui/icons/oxygen/media-seek-forward.png \
    gui/icons/oxygen/media-seek-backward.png \
    gui/icons/oxygen/media-record.png \
    gui/icons/oxygen/media-playback-stop.png \
    gui/icons/oxygen/media-playback-start.png \
    gui/icons/oxygen/media-playback-pause.png \
    gui/icons/oxygen/media-eject.png \
    gui/icons/oxygen/measure.png \
    gui/icons/oxygen/list-remove.png \
    gui/icons/oxygen/list-add.png \
    gui/icons/oxygen/kig.png \
    gui/icons/oxygen/help-hint.png \
    gui/icons/oxygen/help-contextual.png \
    gui/icons/oxygen/help-about.png \
    gui/icons/oxygen/go-up.png \
    gui/icons/oxygen/go-top.png \
    gui/icons/oxygen/games-solve.png \
    gui/icons/oxygen/games-config-board.png \
    gui/icons/oxygen/fork.png \
    gui/icons/oxygen/fill-color.png \
    gui/icons/oxygen/edit-redo.png \
    gui/icons/oxygen/edit-delete.png \
    gui/icons/oxygen/edit-copy.png \
    gui/icons/oxygen/edit-clear-locationbar-rtl.png \
    gui/icons/oxygen/draw-arrow-up.png \
    gui/icons/oxygen/draw-arrow-forward.png \
    gui/icons/oxygen/draw-arrow-down.png \
    gui/icons/oxygen/draw-arrow-back.png \
    gui/icons/oxygen/document-save-as.png \
    gui/icons/oxygen/document-save-all.png \
    gui/icons/oxygen/document-save.png \
    gui/icons/oxygen/document-properties.png \
    gui/icons/oxygen/document-open.png \
    gui/icons/oxygen/document-new.png \
    gui/icons/oxygen/document-export.png \
    gui/icons/oxygen/computer-laptop.png \
    gui/icons/oxygen/computer.png \
    gui/icons/oxygen/camera-web.png \
    gui/icons/oxygen/arrow-up-double.png \
    gui/icons/oxygen/arrow-up.png \
    gui/icons/oxygen/arrow-right-double.png \
    gui/icons/oxygen/arrow-right.png \
    gui/icons/oxygen/arrow-left-double.png \
    gui/icons/oxygen/arrow-left.png \
    gui/icons/oxygen/arrow-down-double.png \
    gui/icons/oxygen/arrow-down.png \
    gui/icons/oxygen/applications-graphics.png \
    gui/icons/oxygen/view-refresh.png

RESOURCES += \
    gui/icons.qrc


QT += opengl

