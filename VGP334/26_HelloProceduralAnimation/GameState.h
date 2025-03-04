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

    FlowerEngine::Graphics::RenderObject mGround;
    FlowerEngine::Physics::RigidBody mGroundRB;
    FlowerEngine::Physics::CollisionShape mGroundShape;

    struct BodyPart
    {
        FlowerEngine::Graphics::RenderObject segment;
        float range = 0.0f;
    };
    std::vector<BodyPart> mBodyParts;
    FlowerEngine::Physics::RigidBody mBodyAnchorRB;
    FlowerEngine::Physics::CollisionShape mBodyAnchorShape;
};