#pragma once

namespace FlowerEngine
{
    enum class ComponentId

    {
        Invalid,        // default value, not valid
        Transform,      // transform component for location data
        Camera,         // contains the Camera for viewing
        FPSCamera,      // move the Camera with FPS controls
        Mesh,           // adds a mesh shape to the game object
        Model,          // adds a model to the game object
        Animator,       // adds an animation controller to the game object
        RigidBody,      // adds a physics body shape to the game object
        SoundEffect,    // adds a sound effect to the object
        SoundBank,      // adds a group of sound effects to an object
        Count           // last value, chain ids in custom components
    };

    enum class ServiceId
    {
        Invalid,        // default value, not valid
        Camera,         // manages the cameras in the world
        Render,         // renders render objects to the world
        Physics,        // manages the physics system in the world
        Count           // last value, chain ids in custom service
    };
}

#define SET_TYPE_ID(id)\
    static uint32_t StaticGetTypedId() { return static_cast<uint32_t>(id); }\
    uint32_t GetTypedId() const override { return StaticGetTypedId(); }