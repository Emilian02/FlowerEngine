 #include "GameState.h"

using namespace FlowerEngine;
using namespace FlowerEngine::Math;
using namespace FlowerEngine::Graphics;
using namespace FlowerEngine::Core;
using namespace FlowerEngine::Input;
using namespace FlowerEngine::Audio;
using namespace FlowerEngine::Physics;

void GameState::Initialize()
{
    mCamera.SetPosition({ 0.0f, 0.0f, 0.0f });
    mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });
    mCamera.SetAspectRatio(0.0f);


    mDirectionalLight.direction = Normalize({ 1.0f, -1.0f, 0.0f });
    mDirectionalLight.ambient = { 0.3f, 0.3f, 0.3f, 1.0f };
    mDirectionalLight.diffuse = { 0.7f, 0.7f, 0.7f, 1.0f };
    mDirectionalLight.specular = { 0.9f, 0.9f, 0.9f, 1.0f };

    std::filesystem::path shaderFile = L"../../Assets/Shaders/Standard.fx";
    mStandardEffect.Intitialize(shaderFile);
    mStandardEffect.SetCamera(mCamera);
    mStandardEffect.SetDirectionalLight(mDirectionalLight);

    mParticleSystemEffect.Initialize();
    mParticleSystemEffect.SetCamera(mCamera);

    PhysicsWorld().Get()->SetGravity({ 0.0f, -2.0f, 0.0f });

    mXBot.Initialize(L"../../Assets/Models/Character04/XBot.model", &mXBotAnimator);
    ModelCache::Get()->AddAnimation(mXBot.modelId, L"../../Assets/Models/Character04/ShovedReactionWithSpin.animset");
    ModelCache::Get()->AddAnimation(mXBot.modelId, L"../../Assets/Models/Character04/FallingDown.animset");
    ModelCache::Get()->AddAnimation(mXBot.modelId, L"../../Assets/Models/Character04/FlipKick.animset");
    mXBotAnimator.Initialize(mXBot.modelId);

    mYBot.Initialize(L"../../Assets/Models/Character02/YBot.model", &mYBotAnimator);
    ModelCache::Get()->AddAnimation(mYBot.modelId, L"../../Assets/Models/Character02/Sitting.animset");
    ModelCache::Get()->AddAnimation(mYBot.modelId, L"../../Assets/Models/Character02/SittingLaughing.animset");
    mYBotAnimator.Initialize(mYBot.modelId);

    Mesh ball = MeshBuilder::CreateSphere(50, 50, 0.2f);
    mBall.meshBuffer.Initialize(ball);
    mBall.diffuseMapId = TextureCache::Get()->LoadTexture("misc/basketball.jpg");

    Mesh cube = MeshBuilder::CreateCube(0.2f);
    mCube.meshBuffer.Initialize(cube);
    mCube.diffuseMapId = TextureCache::Get()->LoadTexture("sprites/green.jpg");
    mCube.transform.position = { 0.5f, 0.5f, -2.5f };

    Mesh ground = MeshBuilder::CreateGroundPlane(25, 25, 1.0f);
    mGround.meshBuffer.Initialize(ground);
    mGround.diffuseMapId = TextureCache::Get()->LoadTexture("misc/concrete.jpg");

    Mesh skySphere = MeshBuilder::CreateSkySphere(200, 200, 200.0f);
    mSkySphere.meshBuffer.Initialize(skySphere);
    mSkySphere.diffuseMapId = TextureCache::Get()->LoadTexture("skysphere/space.jpg");

    mAnimationTime = 0.0f;
    mTotalTimeAnimation = 20.0f;

    LoadAnimationForBall();
    LoadAnimationForXBot();
    LoadAnimationForYBot();
    LoadAnimationForCamera();

    mExplosionEventId = SoundEffectManager::Get()->Load("explosion.wav");
}

void GameState::LoadAnimationForBall()
{
    mBallAnimation = AnimationBuilder()
        .AddPositionKey({ 0.0f, 6.0f, -4.5f }, 0.0f)
        .AddPositionKey({ 0.0f, 6.0f, -4.5f }, 2.0f)
        .AddPositionKey({ 0.0f, 1.5f, -1.2f }, 3.5f)
        .AddPositionKey({ 0.0f, 1.0f, -2.0f }, 4.0f)
        .AddPositionKey({ 0.0f, 0.2f, -2.7f }, 5.0f)
        .AddPositionKey({ 0.0f, 1.5f, -3.4f }, 6.0f)
        .AddPositionKey({ 0.0f, 3.0f, -4.0f }, 7.0f)
        .AddPositionKey({ 0.0f, 2.9f, -4.1f }, 7.1f)
        .AddPositionKey({ 0.0f, 2.8f, -4.2f }, 7.2f)
        .AddPositionKey({ 0.0f, 2.7f, -4.3f }, 7.3f)
        .AddPositionKey({ 0.0f, 2.6f, -4.4f }, 7.4f)
        .AddPositionKey({ 0.0f, 2.2f, -4.8f }, 7.8f)
        .AddPositionKey({ 0.0f, 1.8f, -5.2f }, 8.2f)
        .AddPositionKey({ 0.0f, 1.5f, -5.5f }, 8.5f)
        .AddScaleKey({ 1.0f, 1.0f, 1.0f }, 0.0f)
        .AddScaleKey({ 1.0f, 1.0f, 1.0f }, 3.3f)
        .AddScaleKey({ 1.0f, 0.75f, 1.75f }, 3.4f)
        .AddScaleKey({ 1.0f, 0.5f, 0.5f }, 3.6f)
        .AddScaleKey({ 1.0f, 0.75f, 1.75f }, 3.8f)
        .AddScaleKey({ 1.0f, 1.0f, 1.0f }, 3.9f)
        .AddScaleKey({ 1.0f, 1.0f, 1.0f }, 4.8f)
        .AddScaleKey({ 1.0f, 0.75f, 1.75f }, 4.9f)
        .AddScaleKey({ 1.0f, 0.5f, 0.5f }, 5.1f)
        .AddScaleKey({ 1.0f, 0.75f, 1.75f }, 5.3f)
        .AddScaleKey({ 1.0f, 1.0f, 1.0f }, 5.4f)
        .AddRotationKey(Quaternion::Identity, 0.0f)
        .AddRotationKey(Quaternion::CreateFromAxisAngle(Math::Vector3::XAxis, 90.0f * Math::Constants::DegToRad), 0.5f)
        .AddRotationKey(Quaternion::CreateFromAxisAngle(Math::Vector3::XAxis, 180.0f * Math::Constants::DegToRad), 1.0f)
        .AddRotationKey(Quaternion::CreateFromAxisAngle(Math::Vector3::XAxis, 270.0f * Math::Constants::DegToRad), 1.5f)
        .AddRotationKey(Quaternion::CreateFromAxisAngle(Math::Vector3::XAxis, 360.0f * Math::Constants::DegToRad), 2.0f)
        .AddRotationKey(Quaternion::CreateFromAxisAngle(Math::Vector3::XAxis, 90.0f * Math::Constants::DegToRad), 2.5f)
        .AddRotationKey(Quaternion::CreateFromAxisAngle(Math::Vector3::XAxis, 180.0f * Math::Constants::DegToRad), 3.0f)
        .AddRotationKey(Quaternion::CreateFromAxisAngle(Math::Vector3::XAxis, 270.0f * Math::Constants::DegToRad), 3.5f)
        .AddRotationKey(Quaternion::CreateFromAxisAngle(Math::Vector3::XAxis, 180.0f * Math::Constants::DegToRad), 4.0f)
        .AddRotationKey(Quaternion::CreateFromAxisAngle(Math::Vector3::XAxis, 90.0f * Math::Constants::DegToRad), 4.5f)
        .AddRotationKey(Quaternion::CreateFromAxisAngle(Math::Vector3::XAxis, 360.0f * Math::Constants::DegToRad), 5.5f)
        .AddRotationKey(Quaternion::CreateFromAxisAngle(Math::Vector3::XAxis, 270.0f * Math::Constants::DegToRad), 6.0f)
        .AddRotationKey(Quaternion::CreateFromAxisAngle(Math::Vector3::XAxis, 180.0f * Math::Constants::DegToRad), 6.5f)
        .AddRotationKey(Quaternion::CreateFromAxisAngle(Math::Vector3::XAxis, 90.0f * Math::Constants::DegToRad), 7.0f)
        .AddRotationKey(Quaternion::CreateFromAxisAngle(Math::Vector3::XAxis, 360.0f * Math::Constants::DegToRad), 7.5f)
        .AddRotationKey(Quaternion::CreateFromAxisAngle(Math::Vector3::XAxis, 270.0f * Math::Constants::DegToRad), 8.0f)
        .AddRotationKey(Quaternion::CreateFromAxisAngle(Math::Vector3::XAxis, 180.0f * Math::Constants::DegToRad), 8.5f)
        .AddRotationKey(Quaternion::CreateFromAxisAngle(Math::Vector3::XAxis, 90.0f * Math::Constants::DegToRad), 9.0f)
        .AddEventKey(std::bind(&GameState::EndOfAnimation, this), mTotalTimeAnimation)
        .Build();
}

void GameState::LoadAnimationForXBot()
{
    mAnimationXBot = AnimationBuilder()
        .AddPositionKey({ 0.0f, 0.0f, 0.0f }, 0.0f)
        .AddPositionKey({ 0.0f, 0.0f, 0.0f }, 5.8f)
        .AddPositionKey({ 0.5f, 0.0f, -2.5f }, 5.9f)
        .AddPositionKey({ 0.5f, 0.0f, -2.5f }, 14.0f)
        .AddRotationKey(Quaternion::Identity, 0.0f)
        .AddRotationKey(Quaternion::Identity, 5.8f)
        .AddRotationKey(Quaternion::CreateFromAxisAngle(Math::Vector3::YAxis, 180.0f * Math::Constants::DegToRad), 5.9f)
        .AddEventKey(std::bind(&GameState::StartAnimationXBotEvent, this), 0.0001f)
        .AddEventKey(std::bind(&GameState::OnMoveEvent, this), 3.7f)
        .AddEventKey(std::bind(&GameState::XBotFallingEvent, this), 6.0f)
        .AddEventKey(std::bind(&GameState::XBotBlood, this), 13.0f)
        .AddEventKey(std::bind(&GameState::EndOfAnimation, this), mTotalTimeAnimation)
        .Build();
}

void GameState::LoadAnimationForYBot()
{
    mAnimationYBot = AnimationBuilder()
        .AddPositionKey({ -5.0f, 0.0f, -1.0f }, 0.0f)
        .AddPositionKey({ -5.0f, 0.0f, -1.0f }, 4.0f)
        .AddRotationKey(Quaternion::CreateFromAxisAngle(Math::Vector3::YAxis, -90.0f * Math::Constants::DegToRad), 0.0f)
        .AddEventKey(std::bind(&GameState::StartAnimationYBotEvent, this), 0.0001f)
        .AddEventKey(std::bind(&GameState::EndOfAnimation, this), mTotalTimeAnimation)
        .Build();
}

void GameState::LoadAnimationForCamera()
{
    mCameraAnimationPosition = AnimationBuilder()
        .AddPositionKey({ -6.0f, 2.0f, -6.0f }, 0.0001f)
        .AddPositionKey({ -6.0f, 2.0f, -6.0f }, 12.9f)
        .AddPositionKey({ 0.7f, 1.0f, -2.5f }, 13.0f)
        .AddEventKey(std::bind(&GameState::EndOfAnimation, this), mTotalTimeAnimation)
        .Build();

    mCameraAnimationLookAt = AnimationBuilder()
        .AddPositionKey({ -1.5f, 1.0f, 0.0f }, 0.0001f)
        .AddPositionKey({ -1.5f, 1.0f, 0.0f }, 12.9f)
        .AddPositionKey({ 40.0f, -40.0f, -40.0f }, 13.0f)
        .AddEventKey(std::bind(&GameState::EndOfAnimation, this), mTotalTimeAnimation)
        .Build();
}

void GameState::Terminate()
{
    mSkySphere.Terminate();
    mGround.Terminate();
    mCube.Terminate();
    mBall.Terminate();
    mYBot.Terminate();
    mXBot.Terminate();
    mParticleSystemEffect.Terminate();
    mParticleSystem.Terminate();
    mStandardEffect.Terminate();
}

void GameState::Update(float deltaTime)
{
    InputSystem* input = InputSystem::Get();
    if (mEndOfAnimation || mPauseAnimation)
    {
        UpdateCamera(deltaTime);
    }
    else
    {
        float prevTime = mAnimationTime;
        if (mBallAnimation.GetDuration() > 0.0f)
        {
            mAnimationTime += deltaTime;
            mBallAnimation.PlayEvents(prevTime, mAnimationTime);
            while (mAnimationTime > mBallAnimation.GetDuration())
            {
                mAnimationTime -= mBallAnimation.GetDuration();
            }
        }

        if (mAnimationXBot.GetDuration() > 0.0f)
        {
            mAnimationTime += deltaTime;
            mAnimationXBot.PlayEvents(prevTime, mAnimationTime);
            while (mAnimationTime > mAnimationXBot.GetDuration())
            {
                mAnimationTime -= mAnimationXBot.GetDuration();
            }
        }

        if (mAnimationYBot.GetDuration() > 0.0f)
        {
            mAnimationTime += deltaTime;
            mAnimationYBot.PlayEvents(prevTime, mAnimationTime);
            while (mAnimationTime > mAnimationYBot.GetDuration())
            {
                mAnimationTime -= mAnimationYBot.GetDuration();
            }
        }

        mParticleSystem.Update(deltaTime);

        mXBotAnimator.Update(deltaTime);
        mYBotAnimator.Update(deltaTime);

        mBall.transform = mBallAnimation.GetTransform(mAnimationTime);

        mXBot.transform = mAnimationXBot.GetTransform(mAnimationTime);
        mXBot.transform.position += mOffset;

        mYBot.transform = mAnimationYBot.GetTransform(mAnimationTime);

        mCamera.SetPosition(mCameraAnimationPosition.GetTransform(mAnimationTime).position);
        mCamera.SetLookAt(mCameraAnimationLookAt.GetTransform(mAnimationTime).position);
    }

    if (input->IsKeyPressed(KeyCode::SPACE))
    {
        ResetAnimation();
    }

    if (input->IsKeyPressed(KeyCode::P))
    {
        PauseAnimation();
    }
    if (input->IsKeyPressed(KeyCode::ENTER))
    {
        UnpauseAnimation();
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
}

void GameState::StartAnimationXBotEvent()
{
    mXBotAnimator.PlayAnimation(0, false);
}

void GameState::XBotFallingEvent()
{
    mXBotAnimator.PlayAnimation(1, false);
}

void GameState::XBotBlood()
{
    ParticleSystemInfo info;
    info.textureId = TextureCache::Get()->LoadTexture("sprites/explosion.png");
    info.maxParticles = 100;
    info.particlePerEmit = { 1 , 20 };
    info.delay = 1.0f;
    info.lifeTime = FLT_MAX;
    info.timeBetweenEmit = { 0.2f, 0.4f };
    info.spawnAngle = { -30.0f, 30.0f };
    info.spawnSpeed = { 1.0f, 3.0f };
    info.spawnLifeTime = { 2.0f, 4.0f };
    info.spawnDirection = Math::Vector3::YAxis;
    info.spawnPosition = { 0.5f, 1.0f, -2.5f };
    info.startScale = { Math::Vector3::One, Math::Vector3::One };
    info.endScale = { Math::Vector3::One, Math::Vector3::One };
    info.startColor = { Colors::OrangeRed, Colors::OrangeRed };
    info.endColor = { Colors::OrangeRed, Colors::OrangeRed };
    mParticleSystem.Initialize(info);
}

void GameState::StartAnimationYBotEvent()
{
    mYBotAnimator.PlayAnimation(1, true);
}

void GameState::EndOfAnimation()
{
    mEndOfAnimation = true;
}

void GameState::ResetAnimation()
{
    mEndOfAnimation = false;
    mAnimationTime = 0.0f;
    mOffset -= mOffset;
}

void GameState::PauseAnimation()
{
    mPauseAnimation = true;
}

void GameState::UnpauseAnimation()
{
    mPauseAnimation = false;
}

void GameState::Render()
{
    mParticleSystemEffect.Begin();
        mParticleSystem.Render(mParticleSystemEffect);
    mParticleSystemEffect.End();
    mStandardEffect.Begin();
        mStandardEffect.Render(mXBot);
        mStandardEffect.Render(mYBot);
        mStandardEffect.Render(mGround);
        mStandardEffect.Render(mBall);
        mStandardEffect.Render(mCube);
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