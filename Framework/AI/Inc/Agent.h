#pragma once

#include "Entity.h"

namespace FlowerEngine::AI
{
    class Agent : public Entity
    {
    public:
        Agent(AIWorld& world, uint32_t typeId)
            : Entity(world, typeId)
        {
        }
        ~Agent() override = default;

        const Agent* target = nullptr;
        std::vector<const Agent*> neighbors;
        Math::Vector3 destination = Math::Vector3::Zero;
        Math::Vector3 velocity = Math::Vector3::Zero;
        float maxSpeed = 100.0f;
        float mass = 1.0f;
    };
}