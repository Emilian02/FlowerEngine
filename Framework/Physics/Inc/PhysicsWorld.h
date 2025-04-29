#pragma once

#include "PhysicsDebugDraw.h"

namespace FlowerEngine::Physics
{
    class PhysicsObject;

    class PhysicsWorld final
    {
    public:
        struct Settings
        {
            FlowerEngine::Math::Vector3 gravity = { 0.0f, -9.81f, 0.0f };
            uint32_t simulateSteps = 10;
            float fixedTimeStep = 1.0f / 60.0f;
        };

        static void StaticInitialize(const Settings& settings);
        static void StaticTerminate();
        static PhysicsWorld* Get();

        PhysicsWorld() = default;
        ~PhysicsWorld();

        void Initialize(const Settings& settings);
        void Terminate();

        void Update(float deltaTime);
        void DebugUI();

        void Register(PhysicsObject* physicsObject);
        void Unregister(PhysicsObject* physicsObject);

        const Settings& GetSettings() const;
        void UpdateSettigs(const Settings& settings);
        void SetGravity(const Math::Vector3& gravity);

    private:
        Settings mSettings;

        // bullet object
        btBroadphaseInterface* mInterface = nullptr;
        btCollisionDispatcher* mDispacther = nullptr;
        btDefaultCollisionConfiguration* mCollisionConfiguration = nullptr;
        btSequentialImpulseConstraintSolver* mSolver = nullptr;

        using PhysicsObjects = std::vector<PhysicsObject*>;
        PhysicsObjects mPhysicsObjects;

        PhysicsDebugDraw mPhysicsDebugDraw;
        bool mDebugDraw = false;

        friend class SoftBody;
#ifdef USE_SOFT_BODY
        btSoftRigidDynamicsWorld* mDynamicsWorld = nullptr;
        btSoftRigidDynamicsWorld* GetSoftBodyWorld() { return mDynamicsWorld; }
#else
        btDiscreteDynamicsWorld* mDynamicsWorld = nullptr;
        btSoftRigidDynamicsWorld* GetSoftBodyWorld() { return nullptr; }
#endif
    };
}