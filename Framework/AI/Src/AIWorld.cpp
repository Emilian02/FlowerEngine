#include "Precompiled.h"
#include "AIWorld.h"

using namespace FlowerEngine;
using namespace FlowerEngine::AI;

void AIWorld::Initialize()
{
    // nothing 
}

void AIWorld::Update()
{
    // nothing
}

void AIWorld::Register(Entity* entity)
{
    ASSERT(std::find(mEntities.begin(), mEntities.end(), entity) == mEntities.end(),
        "AIWorld: entity is already added");
}

void AIWorld::Unregister(Entity* entity)
{
    auto iter = std::find(mEntities.begin(), mEntities.end(), entity);
    if (iter != mEntities.end())
    {
        mEntities.erase(iter);
    }
}

//void AIWorld::AddObstacle(const Math::Circle& obstacle)
//{
//    mObstacles.push_back(obstacle);
//}
//
//void AIWorld::AddWall(const Math::LineSegment& wall)
//{
//    mWalls.push_back(wall);
//}
//
//bool AIWorld::HasLineOfSight(const X::Math::LineSegment& lineSegment) const
//{
//    for (const X::Math::LineSegment& wall : mWalls)
//    {
//        if (X::Math::Intersect(lineSegment, wall))
//        {
//            return false;
//        }
//    }
//    for (const X::Math::Circle& obstacle : mObstacles)
//    {
//        if (X::Math::Intersect(lineSegment, obstacle))
//        {
//            return false;
//        }
//    }
//    return true;
//}
//
//EntityPtrs AI::AIWorld::GetEntitiesInRange(const X::Math::Circle& range, uint32_t typeId)
//{
//    const float radiusSqr = range.radius * range.radius;
//    EntityPtrs entities;
//    for (auto& entity : mEntities)
//    {
//        if (typeId == 0 || typeId == entity->GetTypeId())
//        {
//            const X::Math::Vector2 distVec = entity->position - range.center;
//            if (X::Math::MagnitudeSqr(distVec) <= radiusSqr)
//            {
//                entities.push_back(entity);
//            }
//        }
//    }
//
//    return entities;
//}
