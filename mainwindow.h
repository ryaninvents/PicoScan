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
    void cameraSettingsChanged(QCamera *first, QCamera*);

    /// Show a debug image.
    void debugImage(cv::Mat im, QCamera::FrameType type);
    void debugImage(cv::Mat im);



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
    QCamera *testCam;

    /// Testing projector
    QProjector *testProjector;

    /// Image debugger
    ImageViewWidget *dbgIm;

};

#endif // MAINWINDOW_H
