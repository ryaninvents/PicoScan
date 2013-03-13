#ifndef CALIBRATIONDIALOG_H
#define CALIBRATIONDIALOG_H

#include <QDialog>
#include "hardware/camera/qcamera.h"
#include "geom/calibrator.h"

namespace Ui {
class CalibrationDialog;
}
/// Two-pane calibration dialog that allows the user
/// to calibrate a stereo pair.
class CalibrationDialog : public QDialog
{
    Q_OBJECT
    
public:
    /// Create the dialog
    explicit CalibrationDialog(QWidget *parent = 0);
    /// Destroy the dialog
    ~CalibrationDialog();

public slots:
    /// Take a snapshot
    void takeSnap();
    /// Run the calibration algorithm
    void calibrate();
    /// Reset; remove all frames and start over.
    void reset();
    /// Close the dialog.
    void close();
    /// Calibrate the projector.
    void calibrateProjector();
    /// Set left camera
    void setLeft(QCamera *cam);
    /// Show the dialog
    void show();
    /// Left camera has captured a frame
    void leftFrameCaptured(cv::Mat frame,QCamera* cam,QProjector::Pattern *pattern);
    
private:
    /// UI
    Ui::CalibrationDialog *ui;
    /// left camera
    QCamera *left;

    Calibrator *calib;
};

#endif // CALIBRATIONDIALOG_H
