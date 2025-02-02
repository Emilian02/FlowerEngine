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

    // leve fata
    FlowerEngine::Graphics::Camera mCamera;
    FlowerEngine::Graphics::DirectionalLight mDirectionalLight;
    FlowerEngine::Graphics::StandardEffect mStandardEffect;

    // character data
    FlowerEngine::Graphics::RenderGroup mCharacter01;
    FlowerEngine::Graphics::Animator mCharacterAnimator;

    // ground data
    FlowerEngine::Graphics::RenderObject mGround;

    bool mShowSkeleton = false;
    int mAnimationIndex = -1;
};