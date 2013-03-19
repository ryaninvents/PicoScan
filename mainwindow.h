#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPlainTextEdit>

#include "gui/aboutdialog.h"
#include "gui/camerasettingsdialog.h"
#include "gui/calibrationstandarddialog.h"
#include "gui/calibrationdialog.h"
#include "gui/projectorsettingsdialog.h"

#include "hardware/projector/graycodepattern.h"

#include "geom/binarycompiler.h"
#include "geom/sheet.h"

namespace Ui {
class MainWindow;
}
/// Represents the main Scan Studio window.
class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    /// Create a new MainWindow.
    explicit MainWindow(QWidget *parent = 0);
    /// Destroy the MainWindow.
    ~MainWindow();

public slots:

    /// Show the About dialog.
    void showAbout();

    /// Show Scan Studio in fullscreen.
    void setFullScreen(bool fs);

    /// Show the camera settings dialog.
    void showCameraSettings();

    /// Show the projector settings dialog.
    void showProjectorSettings();

    /// Show the calibration settings dialog.
    void showCalStdSettings();

    /// Show the calibration dialog.
    void showCalibrationDialog();

    /// Quit Scan Studio.
    void quitProgram();

    /// Adjust the calibration standard.
    void adjustCalStd();

    /// Take a snapshot.
    void takeFrame();

    /// Write debug info to the side panel.
    void debug(QString str);

    /// Write debug info to the side panel.
    void debug(const char *str);

    /// Show the debug window.
    void showDebug();

    /// User has selected camera settings.
    void cameraSettingsChanged(QCamera *first,
                               QCamera*);

    /// Show a debug image.
    void debugImage(cv::Mat im,
                    QCamera*,
                    QProjector::Pattern*);
    /// Show a debug image.
    void debugImage(cv::Mat im);

    /// Binary image has been computed
    void binaryImageCaptured(cv::Mat binary,bool);

    /// Write debug image 1
    void writeDebugImg1(cv::Mat im);

    /// Write debug image 2
    void writeDebugImg2(cv::Mat im);

    void saveSTL();



protected:
    /// Quit on close.
    void closeEvent(QCloseEvent *);
    
private:

    /// UI
    Ui::MainWindow *ui;

    /// About dialog
    AboutDialog about;

    /// Camera settings dialog
    CameraSettingsDialog camSettings;

    /// Calibration standard dialog
    CalibrationStandardDialog stdSettings;

    /// Calibration dialog with the two views
    CalibrationDialog *calib;

    /// Projector settings dialog
    ProjectorSettingsDialog projSettings;

    /// Enable calibration
    void enableCalibrate();

    /// Debug window
    QPlainTextEdit *debugWin;

    /// Capture camera
    QCamera *capture;

    /// Gray code pattern
    GrayCodePattern *graycode;

    /// Testing camera capture
    QCamera *camera;

    /// Testing projector
    QProjector *projector;

    /// Image debugger
    ImageViewWidget *dbgIm;

    BinaryCompiler *compiler;

    BinaryCompiler *compiler2;

    Sheet *geom;

};

#endif // MAINWINDOW_H
