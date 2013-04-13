#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPlainTextEdit>

#include "gui/aboutdialog.h"
#include "gui/camerasettingsdialog.h"
#include "gui/calibrationstandarddialog.h"
#include "gui/calibrationdialog.h"
#include "gui/projectorsettingsdialog.h"
#include "gui/singlecalibrationdialog.h"

#include "hardware/projector/graycodepattern.h"
#include "hardware/standards/calibrationstandard.h"

#include "geom/binarycompiler.h"
#include "geom/sheet.h"
#include "geom/phasecompiler.h"

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

    void showCamera2Settings();

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

    /// Take a sinusoidal snapshot.
    void phaseMapCaptured(cv::Mat ph, bool);

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

    void hybridImageCaptured(cv::Mat hybrid);

    /// Write debug image 1
    void writeDebugImg1(cv::Mat im);

    /// Write debug image 2
    void writeDebugImg2(cv::Mat im);

    /// Save to an STL file
    void saveSTL();

    /// Set the current scan as background so
    /// it will be removed from subsequent scans.
    void setAsBackground();

    void setShifts(int shifts);
    void setBits(int bits);



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
    QCamera *camera2;

    /// Testing projector
    QProjector *projector;

    /// Image debugger
    ImageViewWidget *dbgIm;

    BinaryCompiler *compiler;

    BinaryCompiler *compiler2;

    Sheet *geom;
    Sheet *bg;

    SingleCalibrationDialog *singleCal;
    SingleCalibrationDialog *singleCal2;

    CalibrationStandard *standard;

    PhaseCompiler *fringer;

    uint sinusoidPower;
    int sinusoidShifts;

    cv::Mat lastBinaryFrame;
    cv::Mat lastPhaseMap;
    cv::Mat lastCombined;

    void computeCombinedGeometry();
};

#endif // MAINWINDOW_H
