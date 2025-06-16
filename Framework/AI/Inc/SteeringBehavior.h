#pragma once

namespace FlowerEngine::AI
{
    class Agent;

    class SteeringBehavior
    {
    public:
        virtual ~SteeringBehavior() = default;

        virtual Math::Vector3 Calculate(Agent& agent) = 0;

        void SetWeight(float weight) { mWeight = weight; }
        float GetWeight() const { return mWeight; }

        void SetActive(bool active) { mActive = active; }
        bool IsActive() const { return mActive; }

        void ShowDebug(bool debug) { mDebug = debug; }
        bool IsDebug() const { return mDebug; }

    private:
        float mWeight = 1.0f;
        bool mActive = false;
        bool mDebug = false;
    };
}