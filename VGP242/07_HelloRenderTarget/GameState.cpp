#include "GameState.h"

using namespace FlowerEngine;
using namespace FlowerEngine::Math;
using namespace FlowerEngine::Graphics;
using namespace FlowerEngine::Core;
using namespace FlowerEngine::Input;

const char* gDrawTypeNames[] =
{
    "None",
    "Transform",
    "GroundPlane",
    "GroudnCircle",
    "Sphere",
    "AABB",
    "AABBFilled",
    "Diamond"
};

void GameState::Initialize()
{
    mCamera.SetPosition({ 0.0f, 1.0f, -3.0f });
    mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });
}

void GameState::Terminate()
{
}

void GameState::Update(float deltaTime)
{
    UpdateCamera(deltaTime);
}

void GameState::UpdateCamera(float deltaTime)
{
    auto input = InputSystem::Get();
    const float moveSpeed = (input->IsKeyDown(KeyCode::LSHIFT) ? 10.0f : 1.0f) * deltaTime;
    const float turnSpeed = 0.1f * deltaTime;
    if (input->IsKeyDown(KeyCode::W))
    {
        mCamera.Walk(moveSpeed);
    }
    else if (input->IsKeyDown(KeyCode::S))
    {
        mCamera.Walk(-moveSpeed);
    }
    if (input->IsKeyDown(KeyCode::D))
    {
        mCamera.Strafe(moveSpeed);
    }
    else if (input->IsKeyDown(KeyCode::A))
    {
        mCamera.Strafe(-moveSpeed);
    }
    if (input->IsKeyDown(KeyCode::E))
    {
        mCamera.Rise(moveSpeed);
    }
    else if (input->IsKeyDown(KeyCode::Q))
    {
        mCamera.Rise(-moveSpeed);
    }
    if (input->IsMouseDown(MouseButton::RBUTTON))
    {
        mCamera.Yaw(input->GetMouseMoveX() * turnSpeed);
        mCamera.Pitch(input->GetMouseMoveY() * turnSpeed);
    }
}

// Geenral Variables
Color color = Colors::AliceBlue;
Vector3 pos = Vector3::Zero;

// Plane Variables
float planeSize = 10;

// Cirlce Variables
float radius = 2.0f;
float slices = 10.0f;
float rings = 10.0f;

//AABB
Vector3 minExtents = Vector3::Zero;
Vector3 maxExtents = Vector3::Zero;

//Diamond
float top = 1.0f;
float bottom = -1.0f; //needs to be negative
float base = 0.5;

void GameState::Render()
{
    if (mDebugDrawType == DebugDrawType::Transform)
    {
        SimpleDraw::AddTransform(Matrix4::Identity);
    }
    else if (mDebugDrawType == DebugDrawType::GroundPlane)
    {
        SimpleDraw::AddGroundPlane(planeSize, color);
    }
    else if (mDebugDrawType == DebugDrawType::GroudnCircle)
    {
        SimpleDraw::AddGroundCircle(slices, radius, pos, color);
    }
    else if (mDebugDrawType == DebugDrawType::Sphere)
    {
        SimpleDraw::AddSphere(slices, rings, radius, pos, color);
    }
    else if (mDebugDrawType == DebugDrawType::AABB)
    {
        SimpleDraw::AddAABB(minExtents, maxExtents, color);
    }
    else if (mDebugDrawType == DebugDrawType::AABBFilled)
    {
        SimpleDraw::AddFilledAABB(minExtents, maxExtents, color);
    }
    else if (mDebugDrawType == DebugDrawType::Diamond)
    {
        SimpleDraw::AddDiamond(top, bottom, base, pos, color);
    }
    SimpleDraw::Render(mCamera);
}

void GameState::DebugUI()
{
    ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    int currentDrawType = static_cast<int>(mDebugDrawType);

    if (ImGui::Combo("DrawType", &currentDrawType, gDrawTypeNames, static_cast<int>(std::size(gDrawTypeNames))))
    {
        mDebugDrawType = (DebugDrawType)currentDrawType;
    }

    if (mDebugDrawType == DebugDrawType::GroundPlane)
    {
        ImGui::DragFloat("Plane Size", &planeSize, 0.1f);
    }
    else if (mDebugDrawType == DebugDrawType::GroudnCircle)
    {
        ImGui::DragFloat("Slices", &slices, 0.1f);
        ImGui::DragFloat("Radius", &radius, 0.1f);
        ImGui::DragFloat3("Position", &pos.x, 0.1f);
    }
    else if (mDebugDrawType == DebugDrawType::Sphere)
    {
        ImGui::DragFloat("Slices", &slices, 0.1f);
        ImGui::DragFloat("Rings", &rings, 0.1f);
        ImGui::DragFloat("Radius", &radius, 0.1f);
        ImGui::DragFloat3("Position", &pos.x, 0.1f);
    }
    else if (mDebugDrawType == DebugDrawType::AABB)
    {
        ImGui::DragFloat3("MinExtents", &minExtents.x, 0.1f);
        ImGui::DragFloat3("MaxExtents", &maxExtents.x, 0.1f);
    }
    else if (mDebugDrawType == DebugDrawType::AABBFilled)
    {
        ImGui::DragFloat3("MinExtents", &minExtents.x, 0.1f);
        ImGui::DragFloat3("MaxExtents", &maxExtents.x, 0.1f);
    }
    else if (mDebugDrawType == DebugDrawType::Diamond)
    {
        ImGui::DragFloat("Top", &top, 0.1f);
        ImGui::DragFloat("Bottom", &bottom, 0.1f);
        ImGui::DragFloat("Base", &base, 0.1f);
        ImGui::DragFloat3("Position", &pos.x, 0.1f);
    }

    ImGui::ColorEdit4("Color", &color.r);
    ImGui::End();
}