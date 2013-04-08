#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <stdio.h>
#include <QScrollBar>
#include <QFileDialog>
#include <opencv2/imgproc/imgproc.hpp>
#include "hardware/camera/qopencvcamera.h"
#include "hardware/projector/seconddisplayprojector.h"
#include "geom/triangulator.h"
#include "hardware/projector/gridpattern.h"
#include "hardware/projector/flatcolorpattern.h"
#include "hardware/standards/dotmatrixstandard.h"
#include "hardware/standards/chessboardstandard.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    calib(new CalibrationDialog),
    debugWin(new QPlainTextEdit),
    graycode(new GrayCodePattern(1,false)),
    dbgIm(new ImageViewWidget),
    singleCal(new SingleCalibrationDialog),
    bg(0),
    singleCal2(new SingleCalibrationDialog),
    sinusoidPower(6),
    sinusoidShifts(8)
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
    dbgIm->setWindowTitle("Debugging -- camera view");

    QOpenCVCamera *capCam = new QOpenCVCamera(2);
    connect(capCam,
            SIGNAL(debug(QString)),
            this,
            SLOT(debug(QString)));
    capCam->setResolution(1600,1200);
    capCam->setPrincipalPoint(800,600);
    capCam->setFocalLength(1300);
    capCam->setPosition(cv::Vec3d(-0.25,-0.01,0));
    capCam->setOrientation(cv::Vec3d(0,M_PI/12,0));

    camera = capCam;

    QOpenCVCamera *capCam2 = new QOpenCVCamera(1);
    connect(capCam2,
            SIGNAL(debug(QString)),
            this,
            SLOT(debug(QString)));
    capCam2->setResolution(1600,1200);
    capCam2->setPrincipalPoint(800,600);
    capCam2->setFocalLength(1300);
    capCam2->setPosition(cv::Vec3d(0.2,0,0));
    capCam2->setOrientation(cv::Vec3d(0,-0.1,0));

    camera2 = capCam2;

    SecondDisplayProjector *pj = new SecondDisplayProjector();
    pj->setScreen(1);
    pj->setResolution(848,480);
    pj->setPrincipalPoint(848/2,480/2);
    pj->setPosition(cv::Vec3d(0.0,0,0));
    pj->setOrientation(cv::Vec3d(-0*M_PI/180,0.0,0.0));
    pj->setFocalLength(3600);

    GridPattern *grid = new GridPattern();
    pj->queue(grid);
//    FlatColorPattern *flat = new FlatColorPattern();
//    pj->queue(flat);

    projector = pj;
    capCam->connectProjector(pj);
    capCam2->connectProjector(pj);


    compiler = new BinaryCompiler(capCam);
    compiler->setProjector(pj);

    fringer = new PhaseCompiler(capCam);
    fringer->setProjector(pj);

    singleCal->setCamera(capCam);
    singleCal2->setCamera(capCam2);

    // debug our components

    connect(pj,
            SIGNAL(debug(QString)),
            this,
            SLOT(debug(QString)));
//    connect(compiler,
//            SIGNAL(visualBinaryFrame(cv::Mat)),
//            this,
//            SLOT(debugImage(cv::Mat)));
    connect(compiler,
            SIGNAL(visualBinaryFrame(cv::Mat)),
            this,
            SLOT(writeDebugImg1(cv::Mat)));
    connect(compiler,
            SIGNAL(binaryFrameCaptured(cv::Mat,bool)),
            this,
            SLOT(binaryImageCaptured(cv::Mat,bool)));
    connect(compiler,
            SIGNAL(debug(QString)),
            this,
            SLOT(debug(QString)));
    connect(calib,
            SIGNAL(debug(QString)),
            this,
            SLOT(debug(QString)));
    connect(fringer, SIGNAL(phaseMapCaptured(cv::Mat,bool)),
            this,    SLOT(phaseMapCaptured(cv::Mat,bool)));

    connect(ui->analysis, SIGNAL(plotCrossSection(QString,cv::Mat)),
            ui->plot,     SLOT(plotLine(QString,cv::Mat)));
    connect(ui->analysis, SIGNAL(cropXChanged(int,int)),
            ui->plot,     SLOT(setXRange(int,int)));



//    standard = new DotMatrixStandard(cv::Size(6,7),
//                                     0.91*20e-3, 0.91*17e-3,
//                                     0.91*10e-3);
    standard = new ChessboardStandard(cv::Size(9,12),20e-3);
    singleCal->setStandard(standard);

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

void MainWindow::debugImage(cv::Mat im,
                            QCamera*,
                            QProjector::Pattern*)
{
    debug("User interface displays image.");
    if(im.channels()==3){
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
    dbgIm->raise();
}

void MainWindow::debugImage(cv::Mat im)
{
    debugImage(im,0,0);
}

void MainWindow::binaryImageCaptured(cv::Mat binary, bool)
{
    // do nothing if the calibration dialog is open
    // hacky; fix later
    if(calib->isVisible()) return;
    binary.convertTo(lastBinaryFrame,CV_64F);
    ui->analysis->setBinary(lastBinaryFrame);
    computeCombinedGeometry();

    geom = Triangulator::computeSheet(
                lastBinaryFrame,
                camera,
                projector,
                1);
//    geom->removeNonManifold();
//    std::cout << "foreground: " << geom << '\n';
//    if(bg) geom->removeBackground(bg,0.005);
    ui->modelView->setData(geom);
}

void MainWindow::writeDebugImg1(cv::Mat im)
{
    cv::imwrite("/home/ryan/Documents/mqp-data/debug/cam1.png",im);
}

void MainWindow::writeDebugImg2(cv::Mat im)
{
    cv::imwrite("/home/ryan/Documents/mqp-data/debug/cam2.png",im);
}

void MainWindow::saveSTL()
{
    QString fnm;
    fnm = QFileDialog::getSaveFileName(
                    this,
                    tr("Save STL"),
                    QDir::homePath(),
                    tr("Standard Tesselation Language(*.stl)"));
    if(fnm.length()>0){
        if(!fnm.endsWith(".stl")){
            fnm = fnm.append(".stl");
        }
        geom->saveSTL(fnm.toLocal8Bit().data());
    }
}

void MainWindow::setAsBackground()
{
    std::cout << "background: " << geom << '\n';
    bg = geom;
    ui->modelView->setBackground(geom);
    ui->modelView->setData(0);
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
    singleCal->open();
}

void MainWindow::showCamera2Settings()
{
    debug("Showing camera 2 settings.");
    singleCal2->open();
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
    calib->setLeft(camera);
    calib->setRight(camera2);
    calib->setProjector(projector);
    calib->setBinary(compiler);
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
    compiler->requestFrame(10);
    fringer->requestFrame(1<<(sinusoidPower-1),sinusoidShifts);
}

void MainWindow::phaseMapCaptured(cv::Mat ph, bool)
{
    lastPhaseMap = ph;
    ui->analysis->setPhaseMap(ph);
    computeCombinedGeometry();
}

void MainWindow::closeEvent(QCloseEvent *)
{
    quitProgram();
}

void MainWindow::enableCalibrate()
{
//    ui->actionCalibrate->setEnabled(true);
}

void MainWindow::computeCombinedGeometry()
{
    return;
    lastCombined = Triangulator::combineBinaryAndPhase(
                lastBinaryFrame,
                lastPhaseMap,
                sinusoidPower);
    geom = Triangulator::computeSheet(
                lastPhaseMap,
                camera,
                projector,
                1);
    ui->modelView->setData(geom);
}
