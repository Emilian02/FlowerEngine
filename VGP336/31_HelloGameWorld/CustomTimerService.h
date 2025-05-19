#pragma once

#include "CustomTypeIds.h"

class CustomerTimerService : public FlowerEngine::Service
{
public:
    SET_TYPE_ID(CustomServiceId::CustomTimer);

    void Update(float deltaTime) override;
    void Register(const CustomTimerComponent* timerComponent);
    void Unregister(const CustomTimerComponent* timerComponent);

private:
    using CustomTimerComponents = std::vector<const CustomTimerComponent*>;
    CustomTimerComponents mtimerComponents;
};