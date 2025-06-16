#pragma once

namespace FlowerEngine::AI
{
    class AIWorld;

    class Entity
    {
    public:
        Entity(AIWorld& world, std::uint32_t typeId);
        virtual ~Entity();

        Entity(const Entity&) = delete;
        Entity(const Entity&&) = delete;
        Entity& operator=(const Entity&) = delete;
        Entity& operator=(const Entity&&) = delete;

        AIWorld& world;
        Math::Vector3 position = Math::Vector3::Zero;
        Math::Vector3 heading = Math::Vector3::ZAxis;
        float radius = 1.0f;

        Math::Matrix4 GetWorldTransform() const;

        uint32_t GetTypeId() const { return static_cast<uint32_t>(mUniqueId >> 32); }
        uint64_t GetUniqueId() const { return mUniqueId; }

    private:
        const uint64_t mUniqueId = 0;
    };
}