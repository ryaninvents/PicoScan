#include "povraycamera.h"

#include <fstream>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

PovRayCamera::PovRayCamera(QObject *parent) :
    QCamera(parent),
    simCellSize(8e-6),
    antialiasing(4)
{
}

bool PovRayCamera::isOpen()
{
    return true;
}

void PovRayCamera::release()
{

}

bool PovRayCamera::requestFrame(QCamera::FrameType type)
{
    FILE *file;
    QString command;

    // write camera parameters to disk
    file = fopen(paramFilename.toLocal8Bit().data(),"w");
    fprintf(file,"/// Scan Studio simulated camera\n"
            "/// If your simulation isn't working, make\n"
            "/// sure you have included this file\n"
            "/// in your .pov setup.\n\n"
            "camera {\n"
            "   up -y*%f\n"
            "   right x*%f\n"
            "   direction z*%f\n"
            "   rotate <%f,%f,%f>\n"
            "   translate <%f,%f,%f>\n"
            "}\n",
            simCellSize*getResolutionV(), // up
            simCellSize*getResolutionU(), // right
            simCellSize*getFocalLength(), // focal
            simRotation[0], // rotate X
            simRotation[1], // rotate Y
            simRotation[2], // rotate Z
            simPosition[0], // translate X
            simPosition[1], // translate Y
            simPosition[2]  // translate Z
            );
    fclose(file);

    // write ini parameters to disk
    file = fopen(iniFilename.toLocal8Bit().data(),"w");
    fprintf(file,
            "Width=%d\nHeight=%d\n-D\n"
            "-GA\n+A%d\n"
            "Input_File_Name=%s\n"
            "Output_File_Name=%s",
            getResolutionU(), // width
            getResolutionV(), // height
            antialiasing,     // anti-aliasing
            sceneFilename.toLocal8Bit().data(), // input file
            renderFilename.toLocal8Bit().data() // output file
            );
    fclose(file);

    // run the simulation; we'll wait
    command = QString("povray %1").arg(iniFilename);
    system(command.toLocal8Bit().data());

    // read the image from disk
    int flags = 1;
    switch(type){
    case FULL_COLOR:
        flags = CV_LOAD_IMAGE_COLOR;
        break;
    case DOUBLE:
    case SIGNED_32:
    case UNSIGNED_16:
        flags = CV_LOAD_IMAGE_GRAYSCALE;
        break;
    }

    cv::Mat frame = cv::imread(
                renderFilename.toLocal8Bit().data(),
                flags);

    // emit the frame!
    emit frameCaptured(frame,type);

}
