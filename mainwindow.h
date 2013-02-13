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

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void showAbout();
    void setFullScreen(bool fs);
    void showCameraSettings();
    void showCalStdSettings();
    void showProjectionScreen();
    void showCalibrationDialog();
    void quitProgram();
    void adjustCalStd();
    void takeFrame();

protected:
    void closeEvent(QCloseEvent *);
    
private:
    Ui::MainWindow *ui;
    AboutDialog about;
    CameraSettingsDialog camSettings;
    CalibrationStandardDialog stdSettings;
    HardwareManager *hardware;
    ScanManager *manager;
    ProjectionScreen *screen;
    CalibrationDialog *calib;

    void enableCalibrate();

    bool showingScreen;
    int binBit;
    bool binInv;
    bool binaryMode;
};

#endif // MAINWINDOW_H
