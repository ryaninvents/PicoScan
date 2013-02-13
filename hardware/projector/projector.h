#ifndef PROJECTOR_H
#define PROJECTOR_H

#include "../opticaldevice.h"
#include "../projector/projectionscreen.h"

class Projector : public OpticalDevice
{
    Q_OBJECT
public:
    Projector(ProjectionScreen *screen);
private:
    ProjectionScreen *screen;
};

#endif // PROJECTOR_H
