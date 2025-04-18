#include "Precompiled.h"
#include "CameraComponent.h"

using namespace FlowerEngine;
using namespace FlowerEngine::Graphics;
using namespace FlowerEngine::Math;

void CameraComponent::Initialize()
{
    //
}

void CameraComponent::Terminate()
{
    //
}

void CameraComponent::DebugUI()
{
    Vector3 pos = mCamera.GetPosition();
    if (ImGui::DragFloat3("Position##Camera", &pos.x, 0.1f))
    {
        mCamera.SetPosition(pos);
    }
}

Graphics::Camera& CameraComponent::GetCamera()
{
    return mCamera;
}

const Graphics::Camera& CameraComponent::GetCamera() const
{
    return mCamera;
}
