#pragma once
#include <FlowerEngine/Inc/FlowerEngine.h>

enum class CustomComponentId
{
    CustomDebugDraw = static_cast<uint32_t>(FlowerEngine::ComponentId::Count)
};

enum class CustomServiceId
{
    CustomDebugDrawDisplay = static_cast<uint32_t>(FlowerEngine::ServiceId::Count)
};