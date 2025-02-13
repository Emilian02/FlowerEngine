#pragma once

#include "PhysicsObject.h"

namespace FlowerEngine::Physics
{
    class SoftBody final : public PhysicsObject
    {
    public:
        SoftBody() = default;
        ~SoftBody();

        void Initialize(Graphics::Mesh& mesh, float mass, const std::vector<uint32_t>& fixedNodeIndices);
        void Terminte();

    private:
        void SyncWithGraphics() override;
        btSoftBody* GetSoftBody() override;

        btSoftBody* mSoftBody = nullptr;
        Graphics::Mesh* mMesh = nullptr;
    };
}