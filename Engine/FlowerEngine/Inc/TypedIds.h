#pragma once

namespace FlowerEngine
{
    enum class ComponentId

    {
        Invalid,        // default value, not valid
        Transform,      // transform component for location data
        Camera,         // contains the Camera for viewing
        FPSCamera,      // move the Camera with FPS controls
        Count           // last value, chain ids in custom components
    };

    enum class ServiceId
    {
        Invalid,        // default value, not valid
        Camera,         // manages the cameras in the world
        Count           // las value, chain ids in custom service
    };
}

#define SET_TYPE_ID(id)\
    static uint32_t StaticGetTypedId() { return static_cast<uint32_t>(id); }\
    uint32_t GetTypedId() const override { return StaticGetTypedId(); }