#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <stdio.h>
#include <QScrollBar>
#include "hardware/camera/povraycamera.h"
#include "hardware/projector/povrayprojector.h"

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
    //showDebug();
    dbgIm->setWindowTitle("Debugging -- camera view");

    codeCam = new BinaryCaptureCamera();
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
    pov->setSimPosition(1,0,-1);
    pov->setSimRotation(0,M_PI*0.25,0);

    testCam = capCam;
    testProjector = pov;

    // debug all images out of testCam
    connect(capCam,
            SIGNAL(frameCaptured(cv::Mat,QCamera::FrameType)),
            this,
            SLOT(debugImage(cv::Mat,QCamera::FrameType)));
    connect(codeCam,
            SIGNAL(intermediateFrame(cv::Mat)),
            this,
            SLOT(debugImage(cv::Mat)));

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
    //    debug("tri->setCaptureCamera(capture);");
}

void MainWindow::debugImage(cv::Mat im,QCamera::FrameType type)
{
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
    debugImage(im,QCamera::DOUBLE);
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
//   tri->getProjector()->queue(graycode);
   codeCam->requestFrame(QCamera::DOUBLE);
    /*testProjector->queue(graycode);
    testCam->requestFrame(QCamera::FULL_COLOR);*/
}

void MainWindow::closeEvent(QCloseEvent *)
{
    quitProgram();
}

void MainWindow::enableCalibrate()
{
    ui->actionCalibrate->setEnabled(true);
}
