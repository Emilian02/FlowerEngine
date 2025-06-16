#pragma once

#include "Component.h"

namespace FlowerEngine
{
    class AIComponent : public Component
    {
    public:
        SET_TYPE_ID(ComponentId::AI);

        void Initialize() override;
        void Terminate() override;
        void Update(float deltaTime) override;
        void DebugUI() override;
        void Deserialize(const rapidjson::Value& value) override;
        void Serialize(rapidjson::Document& doc, rapidjson::Value& value, const rapidjson::Value& original) override;

        void ShowDebug(bool show);
        void SetSeek(bool active);
        void SetSeekWeight(float weight);
        void SetFlee(bool active);
        void SetFleeWeight(float weight);
        void SetArrive(bool active);
        void SetArriveWeight(float weight);
        void SetDestination(const Math::Vector3& destination);


        AI::Agent* GetAgent();
        const AI::Agent* GetAgent() const;

    private:
        std::unique_ptr<AI::Agent> mAgent = nullptr;
        std::unique_ptr<AI::SteeringModule> mSteeringModule = nullptr;
        AI::SeekBehavior* mSeekBehavior = nullptr;
        AI::FleeBehavior* mFleeBehavior = nullptr;
        AI::ArriveBehavior* mArriveBehavior = nullptr;
    };
}