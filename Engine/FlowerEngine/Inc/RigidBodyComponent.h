#pragma once


#include "Component.h"

namespace FlowerEngine
{
    class RigidBodyComponent : public Component
    {
    public:
        SET_TYPE_ID(ComponentId::RigidBody);

        void Initialize() override;
        void Terminate() override;
        void Deserialize(const rapidjson::Value& value) override;

        void SetPosition(const Math::Vector3& position);
        void SetVelocity(const Math::Vector3& velocity);

        float GetMass();

    private:
        friend class PhysicsService;
        Physics::CollisionShape mCollisionShape;
        Physics::RigidBody mRigidBody;
        float mMass = -1.0f;
    };
}