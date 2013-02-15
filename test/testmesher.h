#ifndef TESTMESHER_H
#define TESTMESHER_H

#include <QString>

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

};

#endif // TESTMESHER_H
