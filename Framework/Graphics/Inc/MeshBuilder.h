#pragma once

#include "MeshTypes.h"

namespace FlowerEngine::Graphics
{
    class MeshBuilder
    {
    public:
        // Cube
        static MeshPC CreateCubePC(float size);
        
        // Rectangle
        static MeshPC CreateRectPC(float width, float length, float height);

        // Plane
        static MeshPC CreatePlanePC(float numRows, int numCols, float spacing);
        static MeshPX CreatePlanePX(float numRows, int numCols, float spacing);

        // Cylinder
        static MeshPC CreateCylinderPC(int slices, int rings);

        // Sphere
        static MeshPC CreateSpherePC(int slices, int rings, float radius);
        static MeshPX CreateSpherePX(int slices, int rings, float radius);
    };
}