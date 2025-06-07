#pragma once
#include <FlowerEngine/Inc/FlowerEngine.h>

enum class CustomComponentId
{
    CustomDebugDraw = static_cast<uint32_t>(FlowerEngine::ComponentId::Count),
    CustomTimer = static_cast<uint32_t>(FlowerEngine::ComponentId::Count) + 1
};

enum class CustomServiceId
{
    CustomDebugDrawDisplay = static_cast<uint32_t>(FlowerEngine::ServiceId::Count),
    CustomTimerDisplay = static_cast<uint32_t>(FlowerEngine::ServiceId::Count) + 1
};