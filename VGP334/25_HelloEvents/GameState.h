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
    void StartAnimationEvent();
    void OnSpacePressedEvent(const FlowerEngine::Event& e);
    void OnPKeyPressedEvent(const FlowerEngine::Event& e);

    FlowerEngine::Graphics::Camera mCamera;

    //Character model
    FlowerEngine::Graphics::RenderGroup mCharacter;
    FlowerEngine::Graphics::Animator mCharacterAnimator;

    //Ball
    FlowerEngine::Graphics::RenderObject mBall;

    //Ground
    FlowerEngine::Graphics::RenderObject mGround;


    FlowerEngine::Graphics::DirectionalLight mDirectionalLight;
    FlowerEngine::Graphics::StandardEffect mStandardEffect;

    FlowerEngine::Math::Vector3 mOffset = { 0.0f, 0.0f, 0.0f };
    FlowerEngine::Math::Vector3 mScaleUp = { 0.0f, 0.0f, 0.0f };
    FlowerEngine::ListenerId mSpacePressedEventId = 0;
    FlowerEngine::ListenerId mPKeyEventId = 0;
    FlowerEngine::Audio::SoundId mGunEventId = 0;
    FlowerEngine::Audio::SoundId mExplosionEventId = 0;

    // animation
    FlowerEngine::Graphics::Animation mAnimation;
    FlowerEngine::Graphics::Animation mAnimationCharacter;
    float mAnimationTime = 0.0f;
    float mAnimationCharcterTime = 0.0f;
};