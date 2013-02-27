#ifndef TESTMESHER_H
#define TESTMESHER_H

#include <QString>
#include <opencv2/core/core.hpp>
#include "filesource.h"

/// Class for testing mesh algorithms.
class TestMesher
{
public:
    /// Some unit tests; this is pretty much abandoned.
    static void unit();
    /// Generate a mesh from input data.
    static void testMesh(char *configFilename);
    /// Generate a phase mapping.
    static void testPhase(char *configFilename);
    /// Map phase from a series of images.
    /// \param source the source of files
    /// \param m the number of images
    static cv::Mat calcPhase(FileSource source, uint m);
    /// Write color map to file
    static void writeToHueFile(char *fnm, cv::Mat img, double scale);

};

#endif // TESTMESHER_H
