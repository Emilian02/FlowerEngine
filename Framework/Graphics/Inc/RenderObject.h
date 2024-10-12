#pragma once

#include "MeshBuffer.h"
#include "TextureCache.h"
#include "Transform.h"

namespace FlowerEngine::Graphics
{

    class RenderObject
    {
    public:
        void Terminate();

        Transform transform;
        MeshBuffer meshBuffer;

        TextureId diffuseTextureId;
    };
}