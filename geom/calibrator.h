#ifndef CALIBRATOR_H
#define CALIBRATOR_H

#include <vector>
#include <opencv2/core/core.hpp>
#include "hardware/scanmanager.h"

class Calibrator
{
public:
    Calibrator(ScanManager *m);
    void clearFrames();
    int addImagePair(cv::Mat left, cv::Mat right);
    int addSingleImage(cv::Mat img);

    /** Run the stereo calibration sequence, and find camera parameters. */
    bool runCalibStereo();

    /** Run the mono calibration sequence. */
    bool runCalibMono();

private:
    std::vector<std::vector<cv::Point2f> > imagePointsLeft;
    std::vector<std::vector<cv::Point2f> > imagePointsRight;
    cv::Size leftSize, rightSize;
    ScanManager *manager;
};

#endif // CALIBRATOR_H
