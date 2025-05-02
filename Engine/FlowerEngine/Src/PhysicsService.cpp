#include "Precompiled.h"
#include "PhysicsService.h"

#include "RigidBodyComponent.h"

using namespace FlowerEngine;

void PhysicsService::Update(float deltaTime)
{
    if (mEnabled)
    {
        Physics::PhysicsWorld::Get()->Update(deltaTime);
    }
}

void PhysicsService::DebugUI()
{
    if (mEnabled)
    {
        Physics::PhysicsWorld::Get()->DebugUI();
    }
}

void PhysicsService::Deserialize(const rapidjson::Value& value)
{
    Physics::PhysicsWorld::Settings settings = Physics::PhysicsWorld::Get()->GetSettings();
    if (value.HasMember("Gravity"))
    {
        const auto& gravity = value["Gravity"].GetArray();
        settings.gravity.x = gravity[0].GetFloat();
        settings.gravity.y = gravity[1].GetFloat();
        settings.gravity.z = gravity[2].GetFloat();
        Physics::PhysicsWorld::Get()->SetGravity(settings.gravity);
    }
    if (value.HasMember("SimSteps"))
    {
        settings.simulateSteps = value["SimSteps"].GetFloat();
    }
    if (value.HasMember("FixedTimeStep"))
    {
        settings.simulateSteps = value["FixedTimeStep"].GetFloat();
    }
    Physics::PhysicsWorld::Get()->UpdateSettigs(settings);
}

void PhysicsService::Register(RigidBodyComponent* rigidBodyComponent)
{
    Physics::PhysicsWorld::Get()->Register(&rigidBodyComponent->mRigidBody);
}

void PhysicsService::Unregister(RigidBodyComponent* rigidBodyComponent)
{
    Physics::PhysicsWorld::Get()->Unregister(&rigidBodyComponent->mRigidBody);
}

void PhysicsService::SetEnabled(bool enabled)
{
    mEnabled = enabled;
}
