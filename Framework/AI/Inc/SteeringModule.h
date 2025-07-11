#pragma once

#include "SteeringBehavior.h"

namespace FlowerEngine::AI
{
    class SteeringModule final
    {
    public:
        SteeringModule(Agent& agent);

        Math::Vector3 Calculate();

        template<class NewBehavior>
        NewBehavior* AddBehavior()
        {
            static_assert(std::is_base_of_v<SteeringBehavior, NewBehavior>,
                "SteeringModule: can only add type SteeringBehavior");
            auto& newBehavior = mBehaviors.emplace_back(std::make_unique<NewBehavior>());
            return static_cast<NewBehavior*>(newBehavior.get());
        }

    private:
        Agent& mAgent;
        std::vector<std::unique_ptr<SteeringBehavior>> mBehaviors;
    };
}