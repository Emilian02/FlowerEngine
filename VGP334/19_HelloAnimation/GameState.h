#pragma once

#include <FlowerEngine/Inc/FlowerEngine.h>


class GameState : public FlowerEngine::AppState
{
public: 
    void Initialize() override;
    void Terminate() override;
    void Update(float deltaTime) override;
    void Render() override;
    void DebugUI() override;

protected:
    void UpdateCamera(float deltaTime);

    FlowerEngine::Graphics::Camera mCamera;

    FlowerEngine::Graphics::RenderObject mBall;
    FlowerEngine::Graphics::RenderObject mGround;
    FlowerEngine::Graphics::DirectionalLight mDirectionalLight;
    FlowerEngine::Graphics::StandardEffect mStandardEffect;

    // animation
    FlowerEngine::Graphics::Animation mAnimation;
    float mAnimationTime = 0.0f;
};