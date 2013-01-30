#ifndef CALIBRATOR_H
#define CALIBRATOR_H

#include <vector>
#include <opencv2/core/core.hpp>
#include "gui/calibrationinprogress.h"
#include "hardware/scanmanager.h"

class Calibrator
{
public:
    Calibrator(CalibrationInProgress *dialog,
               ScanManager *m);
    void clearFrames();
    int addImagePair(cv::Mat left, cv::Mat right);

    /** Run the calibration sequence, and find camera parameters. */
    bool runCalib();

private:
    std::vector<std::vector<cv::Point2f> > imagePointsLeft;
    std::vector<std::vector<cv::Point2f> > imagePointsRight;
    cv::Size leftSize, rightSize;
    CalibrationInProgress *progressDialog;
    ScanManager *manager;
};

#endif // CALIBRATOR_H
