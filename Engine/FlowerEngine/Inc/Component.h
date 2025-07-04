#pragma once

#include "TypedIds.h"

namespace FlowerEngine
{
    class GameObject;

    class Component
    {
    public:
        Component() = default;
        virtual ~Component() = default;

        Component(const Component&) = delete;
        Component(const Component&&) = delete;
        Component& operator=(const Component&) = delete;
        Component& operator=(const Component&&) = delete;

        virtual void Initialize() {}
        virtual void Terminate() {}
        virtual void Update(float deltaTime) {}
        virtual void DebugUI() {}

        // saving data to a file
        virtual void Serialize(rapidjson::Document& doc, rapidjson::Value& value, const rapidjson::Value& orirginal) {}
        // reading data from a file
        virtual void Deserialize(const rapidjson::Value& value) {}

        virtual uint32_t GetTypedId() const = 0;

        GameObject& GetOwner() { return *mOwner; }
        const GameObject& GetOwner() const { return *mOwner; }

    private:
        friend class GameObject;
        GameObject* mOwner = nullptr;
    };
}