#include "Precompiled.h"
#include "PhysicsWorld.h"
#include "PhysicsObject.h"

using namespace FlowerEngine;
using namespace FlowerEngine::Physics;

namespace
{
    std::unique_ptr<PhysicsWorld> sPhysicsWorld;
}

void PhysicsWorld::StaticInitialize(const Settings& settings)
{
    ASSERT(sPhysicsWorld == nullptr, "PhysicsWorld: is alreadt initialized");
    sPhysicsWorld = std::make_unique<PhysicsWorld>();
    sPhysicsWorld->Initialize(settings);
}

void PhysicsWorld::StaticTerminate()
{
    if (sPhysicsWorld != nullptr)
    {
        sPhysicsWorld->Terminate();
        sPhysicsWorld.reset();
    }
}

PhysicsWorld* PhysicsWorld::Get()
{
    ASSERT(sPhysicsWorld != nullptr, "PhysicsWorld: must be initialized");
    return sPhysicsWorld.get();
}

PhysicsWorld::~PhysicsWorld()
{
    ASSERT(mDynamicsWorld != nullptr, "PhysicsWorld: must be terminated");
}

void PhysicsWorld::Initialize(const Settings& settings)
{
    mSettings = settings;
    mCollisionConfiguration = new btDefaultCollisionConfiguration();
    mDispacther = new btCollisionDispatcher(mCollisionConfiguration);
    mInterface = new btDbvtBroadphase();
    mSolver = new btSequentialImpulseConstraintSolver();
    mDynamicsWorld = new btDiscreteDynamicsWorld(mDispacther, mInterface, mSolver, mCollisionConfiguration);
    mDynamicsWorld->setGravity(TobtVector3(settings.gravity));
}

void PhysicsWorld::Terminate()
{
    SafeDelete(mDynamicsWorld);
    SafeDelete(mSolver);
    SafeDelete(mInterface);
    SafeDelete(mDispacther);
    SafeDelete(mCollisionConfiguration);
}

void PhysicsWorld::Update(float deltaTime)
{
    mDynamicsWorld->stepSimulation(deltaTime, mSettings.simulateSteps, mSettings.fixedTimeStep);
    for (PhysicsObject* po : mPhysicsObjects)
    {
        po->SyncWithGraphics();
    }
}

void PhysicsWorld::DebugUI()
{
    if (ImGui::CollapsingHeader("Physics", ImGuiTreeNodeFlags_DefaultOpen))
    {

    }
}

void PhysicsWorld::Register(PhysicsObject* physicsObject)
{
    auto iter = std::find(mPhysicsObjects.begin(), mPhysicsObjects.end(), physicsObject);
    if (iter == mPhysicsObjects.end())
    {
        mPhysicsObjects.push_back(physicsObject);
        if (physicsObject->GetRigidBody())
        {
            mDynamicsWorld->addRigidBody(physicsObject->GetRigidBody());
        }
    }
}

void PhysicsWorld::Unregister(PhysicsObject* physicsObject)
{
    auto iter = std::find(mPhysicsObjects.begin(), mPhysicsObjects.end(), physicsObject);
    if (iter != mPhysicsObjects.end())
    {
        if (physicsObject->GetRigidBody())
        {
            mDynamicsWorld->removeRigidBody(physicsObject->GetRigidBody());
        }
        mPhysicsObjects.erase(iter);
    }
}

void PhysicsWorld::SetGravity(const Math::Vector3& gravity)
{
    mSettings.gravity = gravity;
    mDynamicsWorld->setGravity(TobtVector3(gravity));
}
