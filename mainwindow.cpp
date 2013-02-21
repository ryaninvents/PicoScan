#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <stdio.h>
#include <QScrollBar>
#include "hardware/camera/povraycamera.h"
#include "hardware/projector/povrayprojector.h"
#include "hardware/camera/qopencvcamera.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    calib(new CalibrationDialog),
    debugWin(new QPlainTextEdit),
    graycode(new GrayCodePattern(1,false)),
    dbgIm(new ImageViewWidget)
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
    showDebug();
    dbgIm->setWindowTitle("Debugging -- camera view");
    dbgIm->showMaximized();

    //*
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
    capCam->setSimFocalLength(12e-3);//*/

    /*
    QOpenCVCamera *capCam = new QOpenCVCamera(1);
    connect(capCam,
            SIGNAL(debug(QString)),
            this,
            SLOT(debug(QString)));
    capCam->startStream();
    capCam->setResolution(1600,1200);//*/

    PovRayProjector *pov = new PovRayProjector();
    pov->setFilterFilename(
                tr("/home/ryan/Documents/mqp-data/"
                   "simulation/butterfly-valve/"
                   "projector-filter.png"));
    pov->setParamFilename(
                tr("/home/ryan/Documents/mqp-data/"
                   "simulation/butterfly-valve/"
                   "projector.inc"));
    pov->setSimPosition(0.25,0,-1);
    pov->setSimRotation(0,M_PI*0.1,0);


    testProjector = pov;

    // initialize the binary encoding camera
    codedCamera = new BinaryEncodedCamera(0,10);
    codedCamera->setCaptureCamera(capCam);
    codedCamera->setProjector(pov);


    // debug our components

    connect(pov,
            SIGNAL(debug(QString)),
            this,
            SLOT(debug(QString)));
    connect(capCam,
            SIGNAL(debug(QString)),
            this,
            SLOT(debug(QString)));
    connect(codedCamera,
            SIGNAL(debug(QString)),
            this,
            SLOT(debug(QString)));
    connect(codedCamera,
            SIGNAL(frameCaptured(cv::Mat,QCamera::FrameType,QCamera*)),
            this,
            SLOT(debugImage(cv::Mat,QCamera::FrameType,QCamera*)));

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
    QOpenCVCamera *cv = dynamic_cast<QOpenCVCamera*>(first);
    if(!cv) return;
    connect(first,
            SIGNAL(frameCaptured(cv::Mat,QCamera::FrameType,QCamera*)),
            this,
            SLOT(debugImage(cv::Mat,QCamera::FrameType,QCamera*)));
    cv->startStream();
}

void MainWindow::debugImage(cv::Mat im,QCamera::FrameType type, QCamera *)
{
    debug("User interface displays image.");
    if(type==QCamera::FULL_COLOR){
        cv::Mat3b img;
        im.convertTo(img,CV_8UC3);
        dbgIm->displayImage(img,true);
        dbgIm->show();
        return;
    }
    cv::Mat_<double> m;
    im.convertTo(m,CV_64F);
    dbgIm->displayImage(m,true);
    dbgIm->show();
}

void MainWindow::debugImage(cv::Mat im)
{
    if(im.channels()==3)
        debugImage(im,QCamera::FULL_COLOR,0);
    else
        debugImage(im,QCamera::DOUBLE,0);
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
    codedCamera->requestFrame(QCamera::DOUBLE);
}

void MainWindow::closeEvent(QCloseEvent *)
{
    quitProgram();
}

void MainWindow::enableCalibrate()
{
    ui->actionCalibrate->setEnabled(true);
}
