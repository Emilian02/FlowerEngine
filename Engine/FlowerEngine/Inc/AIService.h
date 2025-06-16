#pragma once

#include "Service.h"


namespace FlowerEngine
{
    class AIComponent;

    class AIService final : public Service
    {
    public:
        SET_TYPE_ID(ServiceId::AI);

        void Initialize() override;
        void Terminate() override;
        void Update(float deltaTime) override;

        void RegisterAIEntity(AI::Entity* entity);
        void UnregisterAIEntity(AI::Entity* entity);
        const AI::EntityPtrs& GetAIEntities() const;
        uint32_t GetNextAIId();

        void SpawnAgent();
        void KillAgent();

        std::vector<std::unique_ptr<AIComponent>>& GetAgents();
        const std::vector<std::unique_ptr<AIComponent>>& GetAgents() const;

        AI::AIWorld& GetAIWorld();
        const AI::AIWorld& GetAIWorld() const;

    private:
        std::vector<std::unique_ptr<AIComponent>> scvAgents;
        AI::AIWorld mAIWorld;
    };
}