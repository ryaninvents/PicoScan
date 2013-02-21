#include "povrayprojector.h"

#include <fstream>
#include "graycodepattern.h"

PovRayProjector::PovRayProjector(QObject *parent) :
    QProjector(parent),
    simCellSize(8e-6),
    simFocal(8e-3)
{
    setResolution(640,640);
}

void PovRayProjector::setFilterFilename(QString fnm)
{
    imageFilename = fnm;
}

void PovRayProjector::setParamFilename(QString fnm)
{
    projFilename = fnm;
}

QString PovRayProjector::getFilterFilename()
{
    return imageFilename;
}

void PovRayProjector::setSimPosition(double x, double y, double z)
{
    simPosition = cv::Vec3d(x,y,z);
}

void PovRayProjector::setSimRotation(double x, double y, double z)
{
    simRotation = cv::Vec3d(x,y,z);
}

void PovRayProjector::projectPattern(QProjector::Pattern *pattern)
{
    QImage im;
    FILE *file;
    lastProjected = pattern;
    // save the projector pattern to disk
    im = pattern->generatePattern(getResolutionU(),getResolutionV());
    im.save(imageFilename,"png");
    GrayCodePattern *gray = dynamic_cast<GrayCodePattern*>(pattern);
    if(gray!=0){
        im.save(tr("/home/ryan/gray_%1_%2.png")
                .arg(gray->getBit())
                .arg(gray->isInverted()));
    }

    // write the projector parameters to disk
    file = fopen(projFilename.toLocal8Bit().data(),"w");
    fprintf(file,"/// Scan Studio simulated projector\n"
            "/// If your simulation isn't working, make\n"
            "/// sure you have included this file\n"
            "/// in your .pov setup.\n\n"
            "#declare projX = %f;\n"
            "#declare projY = %f;\n"
            "#declare projZ = %f;\n"
            "#declare projRX = %f;\n"
            "#declare projRY = %f;\n"
            "#declare projRZ = %f;\n"
            "#declare projSlmSize = %f;\n"
            "#declare projFocal = %f;\n"
            "#declare patternImageFile = \"%s\";\n\n",
            simPosition[0], // projX
            simPosition[1], // projY
            simPosition[2], // projZ
            simRotation[0]*180/M_PI, // projRX
            -simRotation[1]*180/M_PI, // projRY
            simRotation[2]*180/M_PI, // projRZ
            simCellSize*getResolutionU(),       // projSlmSize
            simFocal,       // projFocal
            imageFilename.toLocal8Bit().data()  // imageFile
            );
    fprintf(file,
            "light_source { <projX,projY,projZ> color <1,1,1> }\n"
            "mesh {\n"
            "   triangle {\n"
            "       <-.5,-.5,0>, <0.5,-.5,0>, <0.5,0.5,0>\n"
            "       uv_vectors <0,0>, <1,0>, <1,1>\n"
            "   }\n"
            "   triangle {\n"
            "       <-.5,-.5,0>, <0.5,0.5,0>, <-.5,0.5,0>\n"
            "       uv_vectors <0,0>, <1,1>, <0,1>\n"
            "   }\n"
            "   texture {\n"
            "       uv_mapping pigment {\n"
            "           image_map {\n"
            "               png patternImageFile"
            "               filter all 0.9\n"
            "               map_type 0\n"
            "               interpolate 4\n"
            "           }\n"
            "       }\n"
            "   }\n"
            "   scale projSlmSize\n"
            "   translate<0,0,projFocal>\n"
            "   rotate<projRX,projRY,projRZ>\n"
            "   translate<projX,projY,projZ>\n"
            "}\n\n");
    fclose(file);
    // phew! glad that's over
    emit patternProjected(pattern,this);
}
