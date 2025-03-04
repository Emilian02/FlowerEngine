 #include "GameState.h"

using namespace FlowerEngine;
using namespace FlowerEngine::Math;
using namespace FlowerEngine::Graphics;
using namespace FlowerEngine::Core;
using namespace FlowerEngine::Input;
using namespace FlowerEngine::Audio;

void GameState::Initialize()
{
    mCamera.SetPosition({ 0.0f, 2.0f, -5.0f });
    mCamera.SetLookAt({ 0.0f, 1.0f, 0.0f });
    mCamera.SetAspectRatio(0.0f);

    mDirectionalLight.direction = Normalize({ 1.0f, -1.0f, 1.0f });
    mDirectionalLight.ambient = { 0.3f, 0.3f, 0.3f, 1.0f };
    mDirectionalLight.diffuse = { 0.7f, 0.7f, 0.7f, 1.0f };
    mDirectionalLight.specular = { 0.9f, 0.9f, 0.9f, 1.0f };

    std::filesystem::path shaderFile = L"../../Assets/Shaders/Standard.fx";
    mStandardEffect.Intitialize(shaderFile);
    mStandardEffect.SetCamera(mCamera);
    mStandardEffect.SetDirectionalLight(mDirectionalLight);

    Mesh ground = MeshBuilder::CreateGroundPlane(30, 30, 1.0f);
    mGround.meshBuffer.Initialize(ground);
    mGround.diffuseMapId = TextureCache::Get()->LoadTexture("misc/concrete.jpg");
    mGroundShape.InitializeHull({ 15.0f, 0.5f, 15.0f }, { 0.0f, -0.5f, 0.0f });
    mGroundRB.Initialize(mGround.transform, mGroundShape);

    uint32_t numSegements = 15;
    float maxSphereSize = 1.0f;
    float minSphereSize = 0.25f;
    for (uint32_t i = 0; i < numSegements; ++i)
    {
        const float t = static_cast<float>(i) / static_cast<float>(numSegements - 1);
        float sphereSize = Math::Lerp(maxSphereSize, minSphereSize, t);
        Mesh sphere = MeshBuilder::CreateSphere(30, 30, sphereSize);
        BodyPart& newPart = mBodyParts.emplace_back();
        newPart.segment.meshBuffer.Initialize(sphere);
        newPart.segment.diffuseMapId = TextureCache::Get()->LoadTexture("Sprites/green.jpg");
        newPart.range = sphereSize;
    }
    mBodyAnchorShape.InitializeSphere(maxSphereSize);
    mBodyAnchorRB.Initialize(mBodyParts[0].segment.transform, mBodyAnchorShape, 1.0f);
}

void GameState::Terminate()
{
    for (auto& part : mBodyParts)
    {
        part.segment.Terminate();
    }
    mBodyAnchorRB.Terminate();
    mBodyAnchorShape.Terminate();
    mGroundRB.Terminate();
    mGroundShape.Terminate();
    mGround.Terminate();
    mStandardEffect.Terminate();
}

void GameState::Update(float deltaTime)
{
    UpdateCamera(deltaTime);

    InputSystem* input = InputSystem::Get();
    const float speed = 10.0f;
    Vector3 velocity = Vector3::Zero;
    if (input->IsKeyDown(KeyCode::UP))
    {
        velocity.z = 1.0f;
    }
    else if (input->IsKeyDown(KeyCode::DOWN))
    {
        velocity.z = -1.0f;
    }
    else if (input->IsKeyDown(KeyCode::RIGHT))
    {
        velocity.x = 1.0f;
    }
    else if (input->IsKeyDown(KeyCode::LEFT))
    {
        velocity.x = -1.0f;
    }
    if (MagnitudeSqr(velocity) > 0.0f)
    {
        velocity = Normalize(velocity) * speed;
    }
    velocity.y = mBodyAnchorRB.GetVelocity().y;
    if (input->IsKeyPressed(KeyCode::SPACE))
    {
        velocity.y = 10.f;
    }

    mBodyAnchorRB.SetVelocity(velocity);
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

void GameState::Render()
{
    for (size_t i = 1; i < mBodyParts.size(); ++i)
    {
        BodyPart& prevPart = mBodyParts[i - 1];
        BodyPart& part = mBodyParts[i];
        Vector3 dir = part.segment.transform.position - prevPart.segment.transform.position;
        if (MagnitudeSqr(dir) > 0.0f)
        {
            dir = Normalize(dir);
        }
        else
        {
            dir = Vector3::ZAxis;
        }
        part.segment.transform.position = prevPart.segment.transform.position + (dir * prevPart.range);
    }

    mStandardEffect.Begin();
        mStandardEffect.Render(mGround);
        for (auto& part : mBodyParts)
        {
            mStandardEffect.Render(part.segment);
        }
    mStandardEffect.End();
}

void GameState::DebugUI()
{
    ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

    if (ImGui::CollapsingHeader("Light", ImGuiTreeNodeFlags_DefaultOpen))
    {
        if (ImGui::DragFloat3("Direction", &mDirectionalLight.direction.x, 0.001f))
        {
            mDirectionalLight.direction = Normalize(mDirectionalLight.direction);
        }

        ImGui::ColorEdit4("Ambient##Light", &mDirectionalLight.ambient.r);
        ImGui::ColorEdit4("Diffuse##Light", &mDirectionalLight.diffuse.r);
        ImGui::ColorEdit4("Specular##Light", &mDirectionalLight.specular.r);
    }
    ImGui::End();
}