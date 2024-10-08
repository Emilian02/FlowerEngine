#pragma once

#include "MeshBuffer.h"
#include "Texture.h"
#include "Transform.h"

namespace FlowerEngine::Graphics
{

    class RenderObject
    {
    public:
        void Terminate();

        Transform transform;
        MeshBuffer meshBuffer;
        Texture diffuseTexture;
    };
}