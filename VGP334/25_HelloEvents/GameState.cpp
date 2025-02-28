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

    mCharacter.Initialize(L"../../Assets/Models/Character04/XBot.model", &mCharacterAnimator);
    ModelCache::Get()->AddAnimation(mCharacter.modelId, L"../../Assets/Models/Character04/ShovedReactionWithSpin.animset");
    mCharacterAnimator.Initialize(mCharacter.modelId);

    Mesh ball = MeshBuilder::CreateSphere(60, 60, 0.2f);
    mBall.meshBuffer.Initialize(ball);
    mBall.diffuseMapId = TextureCache::Get()->LoadTexture("misc/basketball.jpg");

    Mesh ground = MeshBuilder::CreateGroundPlane(10, 10, 1.0f);
    mGround.meshBuffer.Initialize(ground);
    mGround.diffuseMapId = TextureCache::Get()->LoadTexture("misc/concrete.jpg");

    mAnimationTime = 0.0f;
    mAnimationCharcterTime = 0.0f;

    mAnimationCharacter = AnimationBuilder()
        .AddPositionKey({ 0.0f, 0.0f, 0.0f }, 0.0f)
        .AddEventKey(std::bind(&GameState::StartAnimationEvent, this), 0.1f)
        .AddEventKey(std::bind(&GameState::OnMoveEvent, this), 1.0f)
        .Build();

    mAnimation = AnimationBuilder()
        .AddPositionKey({ 0.0f, 3.0f, -3.0f }, 0.0f)
        .AddPositionKey({ 0.0f, 2.0f, -2.0f }, 0.5f)
        .AddPositionKey({ 0.0f, 1.5f, -1.0f }, 1.0f)
        .AddScaleKey({ 1.0f, 1.0f, 1.0f }, 0.0f)
        .AddScaleKey({ 1.0f, 1.0f, 1.0f }, 1.4f)
        .AddScaleKey({ 1.0f, 1.0f, 0.75f }, 1.5f)
        .AddScaleKey({ 1.0f, 1.0f, 1.0f }, 1.6f)
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
    EventManager* em = EventManager::Get();
    mSpacePressedEventId = em->AddListener(EventType::SpacePressed, std::bind(&GameState::OnSpacePressedEvent, this, std::placeholders::_1));
    mPKeyEventId = em->AddListener(EventType::PKey, std::bind(&GameState::OnPKeyPressedEvent, this, std::placeholders::_1));

    mGunEventId = SoundEffectManager::Get()->Load("photongun1.wav");
    mExplosionEventId = SoundEffectManager::Get()->Load("explosion.wav");
}

void GameState::Terminate()
{
    EventManager::Get() ->RemoveListener(EventType::SpacePressed, mSpacePressedEventId);
    EventManager::Get() ->RemoveListener(EventType::PKey, mPKeyEventId);
    mGround.Terminate();
    mBall.Terminate();
    mCharacter.Terminate();
    mStandardEffect.Terminate();
}

void GameState::Update(float deltaTime)
{
    UpdateCamera(deltaTime);

    if (mAnimation.GetDuration() > 0.0f)
    {
        float prevTime = mAnimationTime;
        mAnimationTime += deltaTime;
        mAnimation.PlayEvents(prevTime, mAnimationTime);
        while (mAnimationTime > mAnimation.GetDuration())
        {
            mAnimationTime -= mAnimation.GetDuration();
        }
    }

    if (mAnimationCharacter.GetDuration() > 0.0f)
    {
        float prevTime = mAnimationCharcterTime;
        mAnimationCharcterTime += deltaTime;
        mAnimationCharacter.PlayEvents(prevTime, mAnimationCharcterTime);
        if (mCharacterAnimator.IsFinished())
        {
            while (mAnimationCharcterTime > mAnimationCharacter.GetDuration())
            {
                mAnimationCharcterTime -= mAnimationCharacter.GetDuration();
            }
        }
    }

    mCharacterAnimator.Update(deltaTime);

    if (InputSystem::Get()->IsKeyPressed(KeyCode::SPACE))
    {
        SpacePressedEvent spe;
        EventManager::Broadcast(spe);
    }

    if (InputSystem::Get()->IsKeyPressed(KeyCode::P))
    {
        PKeyPresssedEvent pKey;
        EventManager::Broadcast(pKey);
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

void GameState::OnMoveEvent()
{
    mOffset.x += 0.5f;
    SoundEffectManager::Get()->Play(mExplosionEventId);
}

void GameState::StartAnimationEvent()
{
    mCharacterAnimator.PlayAnimation(0, false);
}

void GameState::OnSpacePressedEvent(const FlowerEngine::Event& e)
{
    SoundEffectManager::Get()->Play(mGunEventId);
}

void GameState::OnPKeyPressedEvent(const FlowerEngine::Event& e)
{
    mScaleUp += Math::Vector3{ 0.5f, 0.5f, 0.5f };
}

void GameState::Render()
{
    mBall.transform = mAnimation.GetTransform(mAnimationTime);
    mBall.transform.scale += mScaleUp;
    mCharacter.transform = mAnimationCharacter.GetTransform(mAnimationCharcterTime);
    mCharacter.transform.position += mOffset;
    mStandardEffect.Begin();
        AnimationUtil::BoneTransforms boneTranforms;
        AnimationUtil::ComputeBoneTransforms(mCharacter.modelId, boneTranforms, &mCharacterAnimator);
        AnimationUtil::DrawSkeleton(mCharacter.modelId, boneTranforms);
        mStandardEffect.Render(mCharacter);
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