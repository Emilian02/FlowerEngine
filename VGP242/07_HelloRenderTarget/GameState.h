#pragma once

#include <FlowerEngine/Inc/FlowerEngine.h>

enum class DebugDrawType
{
    None,
    Transform,
    GroundPlane,
    GroudnCircle,
    Sphere,
    AABB,
    AABBFilled,
    Diamond
};

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
    DebugDrawType mDebugDrawType = DebugDrawType::None;
};