#pragma once

#include "Entity.h"

namespace FlowerEngine::AI
{
    using EntityPtrs = std::vector<Entity*>;

    class AIWorld
    {
    public:
        /*using Obstacles = std::vector<Math::Circle>;
        using Walls = std::vector<Math::LineSegment>;*/

        void Initialize();
        void Update();

        void Register(Entity* entity);
        void Unregister(Entity* entity);

        /*void AddObstacle(const Math::Circle& obstacle);
        void AddWall(const Math::LineSegment& wall);
        bool HasLineOfSight(const Math::LineSegment& lineSegment) const;

        const Obstacles& GetObstacles() const { return mObstacles; }
        const Walls& GetWalls() const { return mWalls; }*/

        EntityPtrs GetEntities() const { return mEntities; }

        /*EntityPtrs GetEntitiesInRange(const Math::Circle& range, uint32_t typeId);*/
        
        uint32_t GetNextId() const
        {
            ASSERT(mNextId < UINT32_MAX, "AIWorld: ran out of ids");
            return mNextId++;
        }

    private:
        mutable uint32_t mNextId = 0;
        EntityPtrs mEntities;

        /*Obstacles mObstacles;
        Walls mWalls;*/
    };
}