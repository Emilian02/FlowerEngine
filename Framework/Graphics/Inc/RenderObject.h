#pragma once

#include "MeshBuffer.h"
#include "TextureCache.h"
#include "ModelCache.h"
#include "Transform.h"
#include "Material.h"
#include "Animator.h"

namespace FlowerEngine::Graphics
{
    struct Model;

    class RenderObject
    {
    public:
        void Terminate();

        Transform transform;
        MeshBuffer meshBuffer;

        Material material;
        TextureId diffuseMapId;
        TextureId normalMapId;
        TextureId specMapId;
        TextureId bumpMapId;
        TextureId noiseMapId;
    };

    class RenderGroup
    {
    public:
        void Initialize(const std::filesystem::path& modelFilePath, const Animator* anim = nullptr);
        void Initialize(const Model& model, const Animator* anim = nullptr);
        void Terminate();

        ModelId modelId;
        Transform transform;
        const Skeleton* skeleton = nullptr;
        const Animator* animator = nullptr;

        std::vector<RenderObject> renderObjects;
    };
}