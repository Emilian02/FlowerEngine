#pragma once

#include "CustomTypeIds.h"

class CustomTimerComponent : public FlowerEngine::Component
{
public:
    SET_TYPE_ID(CustomComponentId::CustomTimer);

    void Initialize() override;
    void Terminate() override;
    void Update(float deltaTime) override;
    void DebugUI() override;
    void Deserialize(const rapidjson::Value& value) override;


    void ResetTimer();
};