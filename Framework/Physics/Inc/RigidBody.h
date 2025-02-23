#pragma once

#include "PhysicsObject.h"

namespace FlowerEngine::Physics
{
    class CollisionShape;

    class RigidBody final : public PhysicsObject
    {
    public:
        RigidBody() = default;
        ~RigidBody() override;

        void Initialize(FlowerEngine::Graphics::Transform& graphicsTransform, const CollisionShape& shape, float mass = 0.0f, bool addToWorld = true);
        void Terminate();

        void SetPosition(const FlowerEngine::Math::Vector3& position);
        void SetVelocity(const FlowerEngine::Math::Vector3& velocity);

        void Activate();
        void Deactivate();

        bool IsDynamic() const;

    private:
        void SyncWithGraphics() override;
        btRigidBody* GetRigidBody() override;

        btRigidBody* mRigidBody = nullptr;
        btDefaultMotionState* mMotionState = nullptr;
        float mMass = 0.0f;

        FlowerEngine::Graphics::Transform* mGraphicsTransform = nullptr;
    };
}