#ifndef CALIBRATOR_H
#define CALIBRATOR_H

#include <vector>
#include <opencv2/core/core.hpp>

class Calibrator
{
public:
    Calibrator();

    void clearFrames();
    int addImagePair(cv::Mat left, cv::Mat right);
    int addSingleImage(cv::Mat img);

    /** Run the stereo calibration sequence, and find camera parameters. */
    bool runCalibStereo();

    /** Run the mono calibration sequence. */
    bool runCalibMono();

    /** Calibrate the projector assuming the cameras are calibrated. */
    bool addProjectorCalibrationFrame();



private:
    std::vector<std::vector<cv::Point2f> > imagePointsLeft;
    std::vector<std::vector<cv::Point2f> > imagePointsRight;
    std::vector<std::vector<cv::Point3f> > projectorPoints;
    cv::Size leftSize, rightSize;
};

#endif // CALIBRATOR_H
