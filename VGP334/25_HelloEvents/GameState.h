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
    void OnMoveEvent();
    void OnSpacePressedEvent(const FlowerEngine::Event& e);

    FlowerEngine::Graphics::Camera mCamera;

    FlowerEngine::Graphics::RenderObject mBall;
    FlowerEngine::Graphics::RenderObject mGround;
    FlowerEngine::Graphics::DirectionalLight mDirectionalLight;
    FlowerEngine::Graphics::StandardEffect mStandardEffect;

    FlowerEngine::Math::Vector3 mOffset = { 0.0f, 0.0f, 0.0f };
    FlowerEngine::ListenerId mSpacePressedEventId = 0;
    FlowerEngine::Audio::SoundId mGunEventId = 0;
    FlowerEngine::Audio::SoundId mExplosionEventId = 0;

    // animation
    FlowerEngine::Graphics::Animation mAnimation;
    float mAnimationTime = 0.0f;
};