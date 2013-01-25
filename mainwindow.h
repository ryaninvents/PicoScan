#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "gui/aboutdialog.h"
#include "gui/camerasettingsdialog.h"

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
    
private:
    Ui::MainWindow *ui;
    AboutDialog about;
    CameraSettingsDialog camSettings;
};

#endif // MAINWINDOW_H
