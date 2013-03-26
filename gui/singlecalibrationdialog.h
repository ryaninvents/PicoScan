#ifndef SINGLECALIBRATIONDIALOG_H
#define SINGLECALIBRATIONDIALOG_H

#include <QDialog>

#include "hardware/standards/calibrationstandard.h"
#include "hardware/camera/qcamera.h"

namespace Ui {
class SingleCalibrationDialog;
}

class SingleCalibrationDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit SingleCalibrationDialog(QWidget *parent = 0);
    ~SingleCalibrationDialog();

public slots:
    void setStandard(CalibrationStandard *standard);
    void setCamera(QCamera *camera);
    void takeFrame();
    void calibrate();
    void frameCaptured(cv::Mat frame, QCamera *cam, QProjector::Pattern *pattern);

    void open();
    void close();
    
private:
    Ui::SingleCalibrationDialog *ui;

    QCamera *camera;
    CalibrationStandard *standard;

    int numFrames;

    std::vector<std::vector<cv::Point2f> > imagePoints;

};

#endif // SINGLECALIBRATIONDIALOG_H
