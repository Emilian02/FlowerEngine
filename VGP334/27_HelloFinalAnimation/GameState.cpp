 #include "GameState.h"

using namespace FlowerEngine;
using namespace FlowerEngine::Math;
using namespace FlowerEngine::Graphics;
using namespace FlowerEngine::Core;
using namespace FlowerEngine::Input;
using namespace FlowerEngine::Audio;

void GameState::Initialize()
{
    mCamera.SetPosition({ -4.0f, 2.0f, -4.0f });
    mCamera.SetLookAt({ 5.0f, 1.0f, 0.0f });
    mCamera.SetAspectRatio(0.0f);

    mDirectionalLight.direction = Normalize({ 1.0f, -1.0f, 0.0f });
    mDirectionalLight.ambient = { 0.3f, 0.3f, 0.3f, 1.0f };
    mDirectionalLight.diffuse = { 0.7f, 0.7f, 0.7f, 1.0f };
    mDirectionalLight.specular = { 0.9f, 0.9f, 0.9f, 1.0f };

    std::filesystem::path shaderFile = L"../../Assets/Shaders/Standard.fx";
    mStandardEffect.Intitialize(shaderFile);
    mStandardEffect.SetCamera(mCamera);
    mStandardEffect.SetDirectionalLight(mDirectionalLight);

    mXBot.Initialize(L"../../Assets/Models/Character04/XBot.model", &mCharacterAnimator);
    ModelCache::Get()->AddAnimation(mXBot.modelId, L"../../Assets/Models/Character04/ShovedReactionWithSpin.animset");
    mCharacterAnimator.Initialize(mXBot.modelId);

    Mesh ball = MeshBuilder::CreateSphere(60, 60, 0.2f);
    mBall.meshBuffer.Initialize(ball);
    mBall.diffuseMapId = TextureCache::Get()->LoadTexture("misc/basketball.jpg");

    Mesh ground = MeshBuilder::CreateGroundPlane(10, 10, 1.0f);
    mGround.meshBuffer.Initialize(ground);
    mGround.diffuseMapId = TextureCache::Get()->LoadTexture("misc/concrete.jpg");

    Mesh skySphere = MeshBuilder::CreateSkySphere(200, 200, 200.0f);
    mSkySphere.meshBuffer.Initialize(skySphere);
    mSkySphere.diffuseMapId = TextureCache::Get()->LoadTexture("skysphere/space.jpg");

    mAnimationTime = 0.0f;
    mAnimationCharcterTime = 0.0f;

    mAnimationCharacter = AnimationBuilder()
        .AddPositionKey({ 0.0f, 0.0f, 0.0f }, 0.0f)
        .AddPositionKey({ 0.0f, 0.0f, 0.0f }, 4.0f)
        .AddEventKey(std::bind(&GameState::StartAnimationEvent, this), 0.1f)
        .AddEventKey(std::bind(&GameState::OnMoveEvent, this), 1.0f)
        .AddEventKey(std::bind(&GameState::ResetAnimation, this), 4.0f)
        .Build();

    mBallAnimation = AnimationBuilder()
        .AddPositionKey({ 0.0f, 3.0f, -3.0f }, 0.0f)
        .AddPositionKey({ 0.0f, 2.0f, -2.0f }, 0.5f)
        .AddPositionKey({ 0.0f, 1.5f, -1.0f }, 1.0f)
        .AddScaleKey({ 1.0f, 1.0f, 1.0f }, 0.0f)
        .AddScaleKey({ 1.0f, 1.0f, 1.0f }, 0.8f)
        .AddScaleKey({ 1.0f, 1.0f, 0.75f }, 0.9f)
        .AddScaleKey({ 1.0f, 1.0f, 1.0f }, 1.0f)
        .AddRotationKey(Quaternion::Identity, 0.0f)
        .AddRotationKey(Quaternion::CreateFromAxisAngle(Math::Vector3::XAxis, 90.0f * Math::Constants::DegToRad), 0.5f)
        .AddRotationKey(Quaternion::CreateFromAxisAngle(Math::Vector3::XAxis, 180.0f * Math::Constants::DegToRad), 1.0f)
        .AddRotationKey(Quaternion::CreateFromAxisAngle(Math::Vector3::XAxis, 270.0f * Math::Constants::DegToRad), 1.5f)
        .AddRotationKey(Quaternion::CreateFromAxisAngle(Math::Vector3::XAxis, 360.0f * Math::Constants::DegToRad), 2.0f)
        .AddRotationKey(Quaternion::CreateFromAxisAngle(Math::Vector3::XAxis, 90.0f * Math::Constants::DegToRad), 2.5f)
        .AddRotationKey(Quaternion::CreateFromAxisAngle(Math::Vector3::XAxis, 180.0f * Math::Constants::DegToRad), 3.0f)
        .AddRotationKey(Quaternion::CreateFromAxisAngle(Math::Vector3::XAxis, 270.0f * Math::Constants::DegToRad), 3.5f)
        .AddRotationKey(Quaternion::CreateFromAxisAngle(Math::Vector3::XAxis, 360.0f * Math::Constants::DegToRad), 4.0f)
        .Build();

    mExplosionEventId = SoundEffectManager::Get()->Load("explosion.wav");
}

void GameState::Terminate()
{
    mSkySphere.Terminate();
    mGround.Terminate();
    mBall.Terminate();
    mXBot.Terminate();
    mStandardEffect.Terminate();
}

void GameState::Update(float deltaTime)
{
    UpdateCamera(deltaTime);

    if (mBallAnimation.GetDuration() > 0.0f)
    {
        float prevTime = mAnimationTime;
        mAnimationTime += deltaTime;
        mBallAnimation.PlayEvents(prevTime, mAnimationTime);
        while (mAnimationTime > mBallAnimation.GetDuration())
        {
            mAnimationTime -= mBallAnimation.GetDuration();
        }
    }

    if (mAnimationCharacter.GetDuration() > 0.0f)
    {
        float prevTime = mAnimationCharcterTime;
        mAnimationCharcterTime += deltaTime;
        mAnimationCharacter.PlayEvents(prevTime, mAnimationCharcterTime);
        while (mAnimationCharcterTime > mAnimationCharacter.GetDuration())
        {
            mAnimationCharcterTime -= mAnimationCharacter.GetDuration();
        }
    }

    mCharacterAnimator.Update(deltaTime);
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

void GameState::OnMoveEvent()
{
    mOffset.x += 0.5f;
}

void GameState::StartAnimationEvent()
{
    mCharacterAnimator.PlayAnimation(0, false);
}

void GameState::ResetAnimation()
{
    mOffset.x -= mOffset.x;
}

void GameState::Render()
{
    mBall.transform = mBallAnimation.GetTransform(mAnimationTime);
    mXBot.transform = mAnimationCharacter.GetTransform(mAnimationCharcterTime);
    mXBot.transform.position += mOffset;
    mStandardEffect.Begin();
        mStandardEffect.Render(mXBot);
        mStandardEffect.Render(mGround);
        mStandardEffect.Render(mBall);
        mStandardEffect.Render(mSkySphere);
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

    ImGui::DragFloat("Time Animation:", &mAnimationTime);

    ImGui::End();
}