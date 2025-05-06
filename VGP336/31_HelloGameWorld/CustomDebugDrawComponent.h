#pragma once

#include "CustomTypeIds.h"

class CustomDebugDrawComponent : public FlowerEngine::Component
{
public:
    SET_TYPE_ID(CustomComponentId::CustomDebugDraw);

    void Initialize() override;
    void Terminate() override;
    void Deserialize(const rapidjson::Value& value) override;

    void AddDebugDraw() const;

private:
    const FlowerEngine::TransformComponent* mTransformComponent = nullptr;
    FlowerEngine::Math::Vector3 mPosition = FlowerEngine::Math::Vector3::Zero;
    FlowerEngine::Color mColor = FlowerEngine::Colors::White;

    uint32_t mSlices = 0;
    uint32_t mRings = 0;
    float mRadius = 0.0f;
};