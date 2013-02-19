#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <stdio.h>
#include <QScrollBar>
#include "hardware/camera/binarycapturecamera.h"
#include "hardware/camera/povraycamera.h"
#include "hardware/projector/povrayprojector.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    calib(new CalibrationDialog),
    debugWin(new QPlainTextEdit),
    graycode(new GrayCodePattern(5,false))
{
    ui->setupUi(this);

    debugWin->setPlainText(QString("ScanStudio started."));
    debugWin->setWindowTitle(QString("Debugging info"));
    debugWin->setWindowIcon(
                QIcon(tr(":/icons/oxygen/camera-web-64.png")));
    debugWin->setWindowFlags(Qt::WindowStaysOnTopHint);
    debugWin->setReadOnly(true);

    connect(&stdSettings,SIGNAL(accept()),this,SLOT(adjustCalStd()));
    connect(&camSettings,
            SIGNAL(debug(QString)),
            this,
            SLOT(debug(QString)));
    connect(&camSettings,
            SIGNAL(acceptedCameras(QCamera*,QCamera*)),
            this,
            SLOT(cameraSettingsChanged(QCamera*,QCamera*)));

    ui->modelView->zoomFit();
    //showDebug();

    BinaryCaptureCamera *codeCam = new BinaryCaptureCamera();
    codeCam->setBitRange(0,10);

    PovRayCamera *capCam = new PovRayCamera();
    capCam->setParameterFilename(
                tr("/home/ryan/Documents/mqp-data/"
                   "simulation/butterfly-valve/"
                   "camera.inc"));
    capCam->setIniFilename(
                tr("/home/ryan/Documents/mqp-data/"
                   "simulation/butterfly-valve/"
                   "scene.ini"));
    capCam->setRenderFilename(
                tr("/home/ryan/Documents/mqp-data/"
                   "simulation/butterfly-valve/"
                   "sim-out.png"));
    capCam->setSceneFilename(
                tr("/home/ryan/Documents/mqp-data/"
                   "simulation/butterfly-valve/"
                   "valve.pov"));
    capCam->setSimZ(-1);
    capCam->setSimFocalLength(12e-3);

    codeCam->setCapturingCamera(capCam);

    PovRayProjector *pov = new PovRayProjector();
    pov->setFilterFilename(
                tr("/home/ryan/Documents/mqp-data/"
                   "simulation/butterfly-valve/"
                   "projector-filter.png"));
    pov->setParamFilename(
                tr("/home/ryan/Documents/mqp-data/"
                   "simulation/butterfly-valve/"
                   "projector.inc"));

    tri = new MonoTriangulator();
    tri->setEncodingCamera(codeCam);
    tri->setCaptureCamera(capCam);
    tri->setProjector(pov);

}

MainWindow::~MainWindow()
{
    delete ui;
    QCoreApplication::exit(0);

}

void MainWindow::debug(QString str)
{
    debugWin->setPlainText(QString("%1\n%2")
                            .arg(debugWin->toPlainText())
                            .arg(str));
    QScrollBar *sb = debugWin->verticalScrollBar();
    sb->setValue(sb->maximum());
}

void MainWindow::debug(const char *str)
{
    debug(QString(str));
}

void MainWindow::showDebug()
{
    debugWin->show();
}

void MainWindow::cameraSettingsChanged(QCamera *first, QCamera *)
{
    capture = first;
    tri->setCaptureCamera(capture);
    debug("cameraSettingsChanged(QCamera*, QCamera*);");
//    debug("tri->setCaptureCamera(capture);");
}

void MainWindow::showAbout()
{
    about.show();
}

void MainWindow::setFullScreen(bool fs)
{
    if(fs){
        showFullScreen();
    }else{
        showNormal();
    }
}

void MainWindow::showCameraSettings()
{
    debug("Showing camera settings.");
    camSettings.show();
    enableCalibrate();
}

void MainWindow::showProjectorSettings()
{
    projSettings.show();
}

void MainWindow::showCalStdSettings()
{
    debug("Show calibration standard settings.");
    stdSettings.show();
}

void MainWindow::showCalibrationDialog()
{
    calib->show();
}

void MainWindow::quitProgram()
{
    QCoreApplication::exit(0);
}

void MainWindow::adjustCalStd()
{
    enableCalibrate();
}

void MainWindow::takeFrame()
{
    //tri->requestSheet();
    tri->getProjector()->queue(graycode);
    tri->getCaptureCamera()->requestFrame(QCamera::DOUBLE);
}

void MainWindow::closeEvent(QCloseEvent *)
{
    quitProgram();
}

void MainWindow::enableCalibrate()
{
    ui->actionCalibrate->setEnabled(true);
}
