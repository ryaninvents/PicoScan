#include "povraycamera.h"

#include <fstream>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <QDir>

PovRayCamera::PovRayCamera(QObject *parent) :
    QCamera(parent),
    simCellSize(8e-6),
    simFocalLength(8e-3),
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

void PovRayCamera::setSimX(double x)
{
    simPosition[0] = x;
}

void PovRayCamera::setSimY(double y)
{
    simPosition[1] = y;
}

void PovRayCamera::setSimZ(double z)
{
    simPosition[2] = z;
}

void PovRayCamera::setSimOrientation(cv::Vec3d r)
{
    simRotation = r;
}

void PovRayCamera::setSimFocalLength(double f)
{
    simFocalLength = f;
}

void PovRayCamera::setSimCellSize(double c)
{
    simCellSize = c;
}

void PovRayCamera::setAntialiasing(int aa)
{
    antialiasing = aa;
}

void PovRayCamera::setParameterFilename(QString fnm)
{
    paramFilename = fnm;
}

void PovRayCamera::setIniFilename(QString fnm)
{
    iniFilename = fnm;
}

void PovRayCamera::setSceneFilename(QString fnm)
{
    sceneFilename = fnm;
}

void PovRayCamera::setRenderFilename(QString fnm)
{
    renderFilename = fnm;
}

QString PovRayCamera::describe()
{
    return QString(
                "POV-Ray Camera\n"
                "Simulated location (%1, %2, %3)\n"
                "Simulated rotation (%4, %5, %6)\n"
                "Simulated focal length %7 mm\n"
                "Simulated cell size %8 um"
                )
            .arg(simPosition[0])
            .arg(simPosition[1])
            .arg(simPosition[2])
            .arg(simRotation[0])
            .arg(simRotation[1])
            .arg(simRotation[2])
            .arg(simFocalLength*1e3)
            .arg(simCellSize*1e6);
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
            "camera {\n");
    fprintf(file,"   up -y*%f\n",simCellSize*getResolutionV());
    fprintf(file,"   right x*%f\n",simCellSize*getResolutionU());
    fprintf(file,"   direction z*%f\n",simFocalLength);
    fprintf(file,"   rotate <%f,%f,%f>\n",
            simRotation[0], // rotate X
            simRotation[1], // rotate Y
            simRotation[2]  // rotate Z
            );
    fprintf(file,"   translate <%f,%f,%f>\n}\n",
            simPosition[0], // translate X
            simPosition[1], // translate Y
            simPosition[2]  // translate Z
            );
    fclose(file);

    QDir dir(sceneFilename);
    dir.cdUp();
    // write ini parameters to disk
    file = fopen(iniFilename.toLocal8Bit().data(),"w");
    fprintf(file,
            "Width=%d\nHeight=%d\n-D\n"
            "-GA\n+R%d\n+A\n"
            "Input_File_Name=%s\n"
            "Output_File_Name=%s\n"
            "Library_Path=%s",
            getResolutionU(), // width
            getResolutionV(), // height
            antialiasing,     // anti-aliasing
            QDir::toNativeSeparators(
                sceneFilename).toLocal8Bit().data(),  // input file
            QDir::toNativeSeparators(
                renderFilename).toLocal8Bit().data(), // output file
            dir.path().toLocal8Bit().data()           // folder
            );
    fclose(file);

    // run the simulation; we'll wait
    command = QString("povray \"%1\"").arg(iniFilename);
    system(command.toLocal8Bit().data());

    // read the image from disk
    int flags = 1;

    printf("%s\n",renderFilename.toLocal8Bit().data());

    cv::Mat frame;

    if(type==QCamera::FULL_COLOR){
        frame = cv::imread(
                    renderFilename.toLocal8Bit().data());
        emit frameCaptured(frame,type);
        return true;
    }

    frame = cv::imread(
                renderFilename.toLocal8Bit().data(),
                CV_LOAD_IMAGE_GRAYSCALE);

    switch(type){
    case DOUBLE:
        frame.convertTo(frame,CV_64F);
        break;
    case SIGNED_32:
        frame.convertTo(frame,CV_32S);
        break;
    case UNSIGNED_16:
        frame.convertTo(frame,CV_16U);
        break;
    }
    emit frameCaptured(frame,type);
    return true;


    /*
    cv::Mat frame;

    frame = cv::imread(renderFilename.toLocal8Bit().data(),flags);
    if(!frame.empty())
        cv::imwrite(renderFilename.append("1").toLocal8Bit().data(),frame);

    /*= cv::imread(
                renderFilename.toLocal8Bit().data(),
                flags);

    // emit the frame!
    //emit frameCaptured(frame,type);*/

}
