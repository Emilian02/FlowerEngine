#pragma once

#include <FlowerEngine/Inc/FlowerEngine.h>


class EditTemplateState : public FlowerEngine::AppState
{
public:
    void Initialize() override;
    void Terminate() override;
    void Update(float deltaTime) override;
    void Render() override;
    void DebugUI() override;

protected:
    FlowerEngine::GameWorld mGameWorld;
};