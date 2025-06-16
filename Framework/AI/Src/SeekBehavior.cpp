#include "Precompiled.h"
#include "SeekBehavior.h"
#include "Agent.h"

using namespace FlowerEngine;
using namespace FlowerEngine::AI;
using namespace FlowerEngine::Graphics;

Math::Vector3 SeekBehavior::Calculate(Agent& agent)
{
    const Math::Vector3 agentToDest = agent.destination - agent.position;
    if (Math::MagnitudeSqr(agentToDest) <= 1.0f)
    {
        return Math::Vector3::Zero;
    }

    const Math::Vector3 desiredVelocity = Math::Normalize(agentToDest) * agent.maxSpeed;
    const Math::Vector3 seekForce = desiredVelocity - agent.velocity;

    if (IsDebug())
    {
        SimpleDraw::AddLine(agent.position, agent.position + desiredVelocity, Colors::Yellow);
        SimpleDraw::AddLine(agent.position, agent.position + agent.velocity, Colors::Green);
        SimpleDraw::AddGroundCircle(20.0f, 1.0f, agent.destination, Colors::Red);
    }

    return seekForce;
}

Math::Vector3 FleeBehavior::Calculate(Agent& agent)
{
    Math::Vector3 desiredVelocity = Math::Vector3::Zero;
    const Math::Vector3 agentToDest = agent.destination - agent.position;
    if (Math::MagnitudeSqr(agentToDest) <= panicDistance * panicDistance)
    {
         desiredVelocity = -Math::Normalize(agentToDest) * agent.maxSpeed;
    }

    const Math::Vector3 fleeForce = desiredVelocity - agent.velocity;

    if (IsDebug())
    {
        SimpleDraw::AddLine(agent.position, agent.position + desiredVelocity, Colors::Yellow);
        SimpleDraw::AddLine(agent.position, agent.position + agent.velocity, Colors::Green);
        SimpleDraw::AddGroundCircle(20.0f, 1.0f, agent.destination, Colors::Red);
        SimpleDraw::AddGroundCircle(panicDistance, 1.0f, agent.destination, Colors::Pink);
    }

    return fleeForce;
}

Math::Vector3 ArriveBehavior::Calculate(Agent& agent)
{
    const Math::Vector3 agentToDest = agent.destination - agent.position;
    const float distance = Math::Magnitude(agentToDest);
    if (distance <= 1.0f)
    {
        return Math::Vector3::Zero;
    }

    const float decelTweaker = agent.mass * 1.5f;
    const float decelerationScale = (static_cast<float>(deceleration) + 1.0f) * decelTweaker;
    float speed = distance / decelerationScale;
    speed = Math::Min(speed, agent.maxSpeed);

    const Math::Vector3 desiredVelocity = (agentToDest / distance) * speed;
    const Math::Vector3 arriveForce = desiredVelocity - agent.velocity;

    if (IsDebug())
    {
        SimpleDraw::AddLine(agent.position, agent.position + desiredVelocity, Colors::Yellow);
        SimpleDraw::AddLine(agent.position, agent.position + agent.velocity, Colors::Green);
        SimpleDraw::AddGroundCircle(20.0f, 1.0f, agent.destination, Colors::Red);
    }

    return arriveForce;
}
