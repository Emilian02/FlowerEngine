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
    FlowerEngine::Graphics::DirectionalLight mDirectionalLight;
    FlowerEngine::Graphics::StandardEffect mStandardEffect;

    // ball info
    FlowerEngine::Graphics::RenderObject mBall;
    FlowerEngine::Physics::CollisionShape mBallShape;
    FlowerEngine::Physics::RigidBody mBallRB;

    FlowerEngine::Graphics::RenderObject mGround;
    FlowerEngine::Physics::CollisionShape mGroundShape;
    FlowerEngine::Physics::RigidBody mGroundRB;

};