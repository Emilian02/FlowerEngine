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

    // ground info
    FlowerEngine::Graphics::RenderObject mGround;
    FlowerEngine::Physics::CollisionShape mGroundShape;
    FlowerEngine::Physics::RigidBody mGroundRB;

    // clott info
    FlowerEngine::Graphics::RenderObject mCloth;
    FlowerEngine::Graphics::Mesh mClothMesh;
    FlowerEngine::Physics::SoftBody mClothSoftBody;

    FlowerEngine::GameWorld mGameWorld;

    // box info
    struct BoxData
    {
        FlowerEngine::Graphics::RenderObject box;
        FlowerEngine::Physics::CollisionShape boxShape;
        FlowerEngine::Physics::RigidBody boxRB;
    };
    using Boxes = std::vector<BoxData>;
    Boxes mBoxes;
    Boxes mMoreBoxes;
};