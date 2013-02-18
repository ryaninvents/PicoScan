#include "povrayprojector.h"

#include <fstream>

PovRayProjector::PovRayProjector(QObject *parent) :
    QProjector(parent),
    simCellSize(8e-6)
{
}

void PovRayProjector::projectPattern(QProjector::Pattern *pattern)
{
    QImage im;
    FILE *file;
    // save the projector pattern to disk
    im = pattern->generatePattern(getResolutionU(),getResolutionV());
    im.save(imageFilename,"png");

    // write the projector parameters to disk
    file = fopen(projFilename.toLocal8Bit().data(),"w");
    fprintf(file,"/// Scan Studio simulated projector\n"
            "/// If your simulation isn't working, make\n"
            "/// sure you have included this file\n"
            "/// in your .pov setup.\n\n"
            "#declare projX  %f\n"
            "#declare projY  %f\n"
            "#declare projZ  %f\n"
            "#declare projRX %f\n"
            "#declare projRY %f\n"
            "#declare projRZ %f\n"
            "#declare projSlmSize %f\n"
            "#declare projFocal %f\n"
            "#declare imageFile \"%s\"\n\n",
            simPosition[0], // projX
            simPosition[1], // projY
            simPosition[2], // projZ
            simRotation[0], // projRX
            simRotation[1], // projRY
            simRotation[2], // projRZ
            simCellSize*getResolutionU(),       // projSlmSize
            simCellSize*getFocalLength(),       // projFocal
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
            "               png \"filter.png\""
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
}
