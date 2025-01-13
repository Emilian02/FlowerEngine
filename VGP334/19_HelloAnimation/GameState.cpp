 #include "GameState.h"

using namespace FlowerEngine;
using namespace FlowerEngine::Math;
using namespace FlowerEngine::Graphics;
using namespace FlowerEngine::Core;
using namespace FlowerEngine::Input;

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

    Mesh ball = MeshBuilder::CreateSphere(60, 60, 0.5f);
    mBall.meshBuffer.Initialize(ball);
    mBall.diffuseMapId = TextureCache::Get()->LoadTexture("misc/basketball.jpg");

    Mesh ground = MeshBuilder::CreateGroundPlane(10, 10, 1.0f);
    mGround.meshBuffer.Initialize(ground);
    mGround.diffuseMapId = TextureCache::Get()->LoadTexture("misc/concrete.jpg");

    mAnimationTime = 0.0f;

    mAnimation = AnimationBuilder()
        .AddPositionKey({ -3.0f, 2.0f, 0.0f }, 0.0f)
        .AddPositionKey({ -2.5f, 1.5f, 0.0f }, 0.5f)
        .AddPositionKey({ -2.0f, 1.0f, 0.0f }, 1.0f)
        .AddPositionKey({ -1.5f, 0.5f, 0.0f }, 1.5f)
        .AddPositionKey({ -1.0f, 1.0f, 0.0f }, 2.0f)
        .AddPositionKey({ -0.5f, 1.5f, 0.0f }, 2.5f)
        .AddPositionKey({  0.0f, 2.0f, 0.0f }, 3.0f)
        .AddPositionKey({  0.5f, 2.5f, 0.0f }, 3.5f)
        .AddScaleKey({1.0f, 1.0f, 1.0f}, 0.0f)
        .AddScaleKey({1.0f, 1.0f, 1.0f}, 1.4f)
        .AddScaleKey({0.75f, 1.0f, 1.0f}, 1.5f)
        .AddScaleKey({ 1.0f, 1.0f, 1.0f }, 1.6f)
        .AddRotationKey(Quaternion::Identity, 0.0f)
        .AddRotationKey(Quaternion::CreateFromAxisAngle(Math::Vector3::ZAxis, -90.0f * Math::Constants::DegToRad), 0.5f)
        .AddRotationKey(Quaternion::CreateFromAxisAngle(Math::Vector3::ZAxis, -180.0f * Math::Constants::DegToRad), 1.0f)
        .AddRotationKey(Quaternion::CreateFromAxisAngle(Math::Vector3::ZAxis, -270.0f * Math::Constants::DegToRad), 1.5f)
        .AddRotationKey(Quaternion::CreateFromAxisAngle(Math::Vector3::ZAxis, -360.0f * Math::Constants::DegToRad), 2.0f)
        .AddRotationKey(Quaternion::CreateFromAxisAngle(Math::Vector3::ZAxis, -90.0f * Math::Constants::DegToRad), 2.5f)
        .AddRotationKey(Quaternion::CreateFromAxisAngle(Math::Vector3::ZAxis, -180.0f * Math::Constants::DegToRad), 3.0f)
        .AddRotationKey(Quaternion::CreateFromAxisAngle(Math::Vector3::ZAxis, -270.0f * Math::Constants::DegToRad), 3.5f)
        .AddRotationKey(Quaternion::CreateFromAxisAngle(Math::Vector3::ZAxis, -360.0f * Math::Constants::DegToRad), 4.0f)
        .Build();
}

void GameState::Terminate()
{
    mGround.Terminate();
    mBall.Terminate();
    mStandardEffect.Terminate();
}

void GameState::Update(float deltaTime)
{
    UpdateCamera(deltaTime);

    if (mAnimation.GetDuration() > 0.0f)
    {
        mAnimationTime += deltaTime;
        while (mAnimationTime > mAnimation.GetDuration())
        {
            mAnimationTime -= mAnimation.GetDuration();
        }
    }
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
    mBall.transform = mAnimation.GetTransform(mAnimationTime);
    mStandardEffect.Begin();
        mStandardEffect.Render(mGround);
        mStandardEffect.Render(mBall);
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