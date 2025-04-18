#pragma once

#include "TypedIds.h"

namespace FlowerEngine
{
    class GameWorld;

    class Service
    {
    public:
        Service() = default;
        virtual ~Service() = default;

        Service(const Service&) = delete;
        Service(const Service&&) = delete;
        Service& operator=(const Service&) = delete;
        Service& operator=(const Service&&) = delete;

        virtual uint32_t GetTypedId() const = 0;

        virtual void Initialize() {};
        virtual void Terminate() {};
        virtual void Update(float deltaTime) {};
        virtual void Render() {};
        virtual void DebugUi() {};

        GameWorld& GetWorld() { return *mWorld; }
        const GameWorld& GetWorld() const { return *mWorld; }

    private:
        friend class GameWorld;
        GameWorld* mWorld = nullptr;
    };
}