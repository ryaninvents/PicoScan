#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "gui/aboutdialog.h"
#include "gui/camerasettingsdialog.h"
#include "gui/calibrationstandarddialog.h"
#include "gui/calibrationdialog.h"

#include "hardware/hardwaremanager.h"
#include "hardware/scanmanager.h"
#include "hardware/projector/projectionscreen.h"

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

    /// Enable calibration
    void enableCalibrate();

};

#endif // MAINWINDOW_H
