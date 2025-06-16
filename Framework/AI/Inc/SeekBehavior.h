#pragma once

#include "SteeringBehavior.h"

namespace FlowerEngine::AI
{
    class SeekBehavior : public SteeringBehavior
    {
    public:
        Math::Vector3 Calculate(Agent& agent) override;
    };

    class FleeBehavior : public SteeringBehavior
    {
    public:
        Math::Vector3 Calculate(Agent& agent) override;

        float panicDistance = 100.0f;
    };
    class ArriveBehavior : public SteeringBehavior
    {
    public:
        enum class Deceleration
        {
            Fast,
            Normal,
            Slow
        };

        Math::Vector3 Calculate(Agent& agent) override;

        Deceleration deceleration = Deceleration::Normal;
    };
}