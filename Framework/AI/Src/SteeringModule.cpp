#include "Precompiled.h"
#include "SteeringModule.h"
#include "Agent.h"

using namespace FlowerEngine;
using namespace FlowerEngine::AI;

SteeringModule::SteeringModule(Agent& agent)
    : mAgent(agent)
{
}

Math::Vector3 SteeringModule::Calculate()
{
    Math::Vector3 totalForce = Math::Vector3::Zero;
    for (auto& behavior : mBehaviors)
    {
        if (behavior->IsActive())
        {
            totalForce += behavior->Calculate(mAgent) * behavior->GetWeight();
        }
    }

    float totalForceSqr = Math::MagnitudeSqr(totalForce);
    if (totalForceSqr > mAgent.maxSpeed * mAgent.maxSpeed)
    {
        totalForce = Math::Normalize(totalForce) * mAgent.maxSpeed;
    }
    else if (totalForceSqr <= 1.0f)
    {
        totalForce = -mAgent.velocity;
    }

    return totalForce;
}
