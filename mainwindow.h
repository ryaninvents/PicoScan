#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "gui/aboutdialog.h"
#include "gui/camerasettingsdialog.h"
#include "gui/calibrationstandarddialog.h"

#include "hardware/scanmanager.h"

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
    
private:
    Ui::MainWindow *ui;
    AboutDialog about;
    CameraSettingsDialog camSettings;
    CalibrationStandardDialog stdSettings;
    ScanManager *manager;
};

#endif // MAINWINDOW_H
