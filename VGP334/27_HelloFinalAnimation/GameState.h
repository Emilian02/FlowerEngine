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
    void ResetAnimation();

    FlowerEngine::Graphics::Camera mCamera;

    //Character model
    FlowerEngine::Graphics::RenderGroup mXBot;
    FlowerEngine::Graphics::Animator mCharacterAnimator;

    //Ball
    FlowerEngine::Graphics::RenderObject mBall;

    //Ground
    FlowerEngine::Graphics::RenderObject mGround;

    //SkySphere
    FlowerEngine::Graphics::RenderObject mSkySphere;

    //Light
    FlowerEngine::Graphics::DirectionalLight mDirectionalLight;
    FlowerEngine::Graphics::StandardEffect mStandardEffect;

    FlowerEngine::Math::Vector3 mOffset = { 0.0f, 0.0f, 0.0f };
    FlowerEngine::Audio::SoundId mExplosionEventId = 0;

    //Animation
    FlowerEngine::Graphics::Animation mBallAnimation;
    FlowerEngine::Graphics::Animation mAnimationCharacter;
    float mAnimationTime = 0.0f;
    float mAnimationCharcterTime = 0.0f;
};