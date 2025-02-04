#include "Precompiled.h"
#include "CollisionShape.h"

using namespace FlowerEngine;
using namespace FlowerEngine::Physics;

CollisionShape::~CollisionShape()
{
    ASSERT(mCollisionShape == nullptr, "CollisionShape: must be called");
}

void CollisionShape::InitializeEmpty()
{
    ASSERT(mCollisionShape == nullptr, "CollisionShape: shape is already initialized");
    mCollisionShape = new btEmptyShape();
}

void CollisionShape::InitializeSphere(float radius)
{
    ASSERT(mCollisionShape == nullptr, "CollisionShape: shape is already initialized");
    mCollisionShape = new btSphereShape(radius);
}
void CollisionShape::InitializeCapsule(float radius, float height)
{
    ASSERT(mCollisionShape == nullptr, "CollisionShape: shape is already initialized");
    mCollisionShape = new btCapsuleShape(radius, height);
}

void CollisionShape::InitializeBox(const FlowerEngine::Math::Vector3& halfExtents)
{
    ASSERT(mCollisionShape == nullptr, "CollisionShape: shape is already initialized");
    mCollisionShape = new btBoxShape(TobtVector3(halfExtents));
}

void CollisionShape::InitializeHull(const FlowerEngine::Math::Vector3& halfExtents, const FlowerEngine::Math::Vector3& origin)
{
    ASSERT(mCollisionShape == nullptr, "CollisionShape: shape is already initialized");
    btConvexHullShape* hullShape = new btConvexHullShape();
    std::vector<FlowerEngine::Math::Vector3> points =
    {
        { -halfExtents.x, -halfExtents.y, -halfExtents.z },
        { -halfExtents.x,  halfExtents.y, -halfExtents.z },
        {  halfExtents.x,  halfExtents.y, -halfExtents.z },
        {  halfExtents.x, -halfExtents.y, -halfExtents.z },
        { -halfExtents.x, -halfExtents.y,  halfExtents.z },
        { -halfExtents.x,  halfExtents.y,  halfExtents.z },
        {  halfExtents.x,  halfExtents.y,  halfExtents.z },
        {  halfExtents.x, -halfExtents.y,  halfExtents.z }
    };

    for (FlowerEngine::Math::Vector3& point : points)
    {
        hullShape->addPoint(TobtVector3(point + origin), false);
    }
    hullShape->recalcLocalAabb();
    mCollisionShape = hullShape;
}

void CollisionShape::Terminate()
{
    SafeDelete(mCollisionShape);
}