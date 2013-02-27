#ifndef FILESOURCE_H
#define FILESOURCE_H

#include <QDir>
#include <opencv2/highgui/highgui.hpp>

/// A source of input files
class FileSource
{
public:

    /// Describes the purpose of an image---gray code, calibration,
    /// or sinusoid capture.
    enum ImageType{
        /// gray code
        GRAY,
        /// calibration
        CALIBRATION,
        /// sinusoid
        SINUSOID
    };

    /// Create a file source
    FileSource();

    /// Set the directory
    void setDirectory(QString str);

    /// Get the desired Gray code image, averaged
    cv::Mat getAveragedGrayCodeImage(uint bit, bool flipped);

    /// Get the desired sinusoid image, averaged
    cv::Mat getAveragedSinusoid(uint shift);

    /// Get the desired calibration image
    cv::Mat getCalibrationImage(uint idx);

    /// Set the averaging
    void setAveraging(uint averaging);

private:
    /// Directory where the files are kept
    QDir fileDirectory;

    /// Number of frames to average together
    uint avg;

    /// Get an image given a directory and index
    cv::Mat loadImageFrom(QDir dir,
                          uint idx);

    /// Get an index given a pattern
    uint getIndexFromPattern(FileSource::ImageType ptype,
                             uint param1,
                             uint param2,
                             uint param3);
};

#endif // FILESOURCE_H
