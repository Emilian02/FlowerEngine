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

    // XBot Events
    void XBotTPose();
    void OnMoveEvent();
    void StartAnimationXBotEvent();
    void XBotFallingEvent();
    void XBotBlood();
    void XBotTurnOffBlood();
    void XBotStandsUp();
    void XBotFlipKick();
    void XBotWalking();
    void XBotVictoryDance();

    // YBot Events
    void StartAnimationYBotEvent();
    void YBotLaughingSitting();

    // Animations Events
    void EndOfAnimation();
    void PauseAnimation();
    void UnpauseAnimation();
    void ResetAnimation();

    // Loading animations
    void LoadAnimationForBall();
    void LoadAnimationForXBot();
    void LoadAnimationForYBot();
    void LoadAnimationForCamera();

    bool mEndOfAnimation = false;
    bool mPauseAnimation = false;
    bool mTurnParticles = false;
    float mTotalTimeAnimation = 0.0f;

    FlowerEngine::Graphics::Camera mCamera;

    //Character model
    FlowerEngine::Graphics::RenderGroup mXBot;
    FlowerEngine::Graphics::Animator mXBotAnimator;

    FlowerEngine::Graphics::RenderGroup mYBot;
    FlowerEngine::Graphics::Animator mYBotAnimator;

    //Ball
    FlowerEngine::Graphics::RenderObject mBall;

    // Sun
    FlowerEngine::Graphics::RenderObject mSun;

    // YBotIndicator
    FlowerEngine::Graphics::RenderObject mYBotIndicator;

    //Ground
    FlowerEngine::Graphics::RenderObject mGround;

    //SkySphere
    FlowerEngine::Graphics::RenderObject mSkySphere;

    //Light
    FlowerEngine::Graphics::DirectionalLight mDirectionalLight;
    FlowerEngine::Graphics::StandardEffect mStandardEffect;

    // Particles
    FlowerEngine::Graphics::ParticleSystemEffect mParticleSystemEffect;
    FlowerEngine::Physics::ParticleSystem mParticleSystem;

    FlowerEngine::Math::Vector3 mOffset = { 0.0f, 0.0f, 0.0f };
    FlowerEngine::Audio::SoundId mExplosionEventId = 0;

    //Animation
    FlowerEngine::Graphics::Animation mBallAnimation;
    FlowerEngine::Graphics::Animation mAnimationXBot;
    FlowerEngine::Graphics::Animation mAnimationYBot;
    FlowerEngine::Graphics::Animation mAnimationYBotIndicator;
    FlowerEngine::Graphics::Animation mCameraAnimationPosition;
    FlowerEngine::Graphics::Animation mCameraAnimationLookAt;

    FlowerEngine::GameWorld mGameWorld;

    //Animation Time
    float mAnimationTime = 0.0f;
};