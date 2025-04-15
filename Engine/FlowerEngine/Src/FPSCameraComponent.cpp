#include "Precompiled.h"
#include "FPSCameraComponent.h"

#include "CameraComponent.h"
#include "GameObject.h"

using namespace FlowerEngine;
using namespace FlowerEngine::Graphics;
using namespace FlowerEngine::Input;
using namespace FlowerEngine::Math;

void FPSCameraComponet::Initialize()
{
    mCameraComponent = GetOwner().GetComponent<CameraComponent>();
    ASSERT(mCameraComponent != nullptr, "FPSCameraComponent: camera was not found");
}

void FPSCameraComponet::Terminate()
{
    mCameraComponent = nullptr;
}

void FPSCameraComponet::Update(float deltaTime)
{
    Camera& camera = mCameraComponent->GetCamera();
    auto input = InputSystem::Get();
    const float moveSpeed = (input->IsKeyDown(KeyCode::LSHIFT) ?mShiftSpeed : mMoveSpeed) * deltaTime;
    const float turnSpeed = mTurnSpeed * deltaTime;
    if (input->IsKeyDown(KeyCode::W))
    {
        camera.Walk(moveSpeed);
    }
    else if (input->IsKeyDown(KeyCode::S))
    {
        camera.Walk(-moveSpeed);
    }
    if (input->IsKeyDown(KeyCode::D))
    {
        camera.Strafe(moveSpeed);
    }
    else if (input->IsKeyDown(KeyCode::A))
    {
        camera.Strafe(-moveSpeed);
    }
    if (input->IsKeyDown(KeyCode::E))
    {
        camera.Rise(moveSpeed);
    }
    else if (input->IsKeyDown(KeyCode::Q))
    {
        camera.Rise(-moveSpeed);
    }
    if (input->IsMouseDown(MouseButton::RBUTTON))
    {
        camera.Yaw(input->GetMouseMoveX() * turnSpeed);
        camera.Pitch(input->GetMouseMoveY() * turnSpeed);
    }
}
