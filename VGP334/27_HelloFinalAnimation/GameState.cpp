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
    mGameWorld.AddService<PhysicsService>();
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
    ModelCache::Get()->AddAnimation(mXBot.modelId, L"../../Assets/Models/Character04/GettingUp.animset");
    ModelCache::Get()->AddAnimation(mXBot.modelId, L"../../Assets/Models/Character04/FlipKick.animset");
    ModelCache::Get()->AddAnimation(mXBot.modelId, L"../../Assets/Models/Character04/Walking.animset");
    ModelCache::Get()->AddAnimation(mXBot.modelId, L"../../Assets/Models/Character04/BreakdanceFreeze.animset");
    mXBotAnimator.Initialize(mXBot.modelId);

    mYBot.Initialize(L"../../Assets/Models/Character02/YBot.model", &mYBotAnimator);
    ModelCache::Get()->AddAnimation(mYBot.modelId, L"../../Assets/Models/Character02/Sitting.animset");
    ModelCache::Get()->AddAnimation(mYBot.modelId, L"../../Assets/Models/Character02/SittingLaughing.animset");
    mYBotAnimator.Initialize(mYBot.modelId);

    Mesh ball = MeshBuilder::CreateSphere(50, 50, 0.2f);
    mBall.meshBuffer.Initialize(ball);
    mBall.diffuseMapId = TextureCache::Get()->LoadTexture("misc/basketball.jpg");

    Mesh sun = MeshBuilder::CreateSphere(50, 50, 50.0f);
    mSun.meshBuffer.Initialize(sun);
    mSun.diffuseMapId = TextureCache::Get()->LoadTexture("planets/sun.jpg");
    mSun.transform.position = { -250.0f, 250.0f, 0.0f };

    Mesh indicator = MeshBuilder::CreateSphere(50, 50, 1.0f);
    mYBotIndicator.meshBuffer.Initialize(indicator);
    mYBotIndicator.diffuseMapId = TextureCache::Get()->LoadTexture("sprites/blue.jpg");
    mYBotIndicator.transform.position = { 0.0f, -10.0f, 0.0f };

    Mesh ground = MeshBuilder::CreateGroundPlane(25, 25, 1.0f);
    mGround.meshBuffer.Initialize(ground);
    mGround.diffuseMapId = TextureCache::Get()->LoadTexture("misc/concrete.jpg");

    Mesh skySphere = MeshBuilder::CreateSkySphere(200, 200, 500.0f);
    mSkySphere.meshBuffer.Initialize(skySphere);
    mSkySphere.diffuseMapId = TextureCache::Get()->LoadTexture("skysphere/space.jpg");

    mAnimationTime = 0.0f;
    mTotalTimeAnimation = 45.0f;

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
        .AddPositionKey({ 0.0f, 6.0f, -4.5f }, 0.8f)
        .AddPositionKey({ 0.0f, 1.5f, -1.2f }, 1.2f)
        .AddPositionKey({ 0.0f, 1.0f, -2.0f }, 1.6f)
        .AddPositionKey({ 0.0f, 0.2f, -2.7f }, 2.0f)
        .AddPositionKey({ 0.0f, 1.5f, -3.4f }, 2.5f)
        .AddPositionKey({ 0.0f, 3.0f, -4.0f }, 3.0f)
        .AddPositionKey({ 0.0f, 2.9f, -4.1f }, 3.1f)
        .AddPositionKey({ 0.0f, 2.8f, -4.2f }, 3.2f)
        .AddPositionKey({ 0.0f, 2.7f, -4.3f }, 3.3f)
        .AddPositionKey({ 0.0f, 2.6f, -4.4f }, 3.4f)
        .AddPositionKey({ 0.0f, 2.2f, -4.8f }, 3.8f)
        .AddPositionKey({ 0.0f, 1.8f, -5.2f }, 4.2f)
        .AddPositionKey({ 0.0f, 1.5f, -5.5f }, 4.5f)
        .AddPositionKey({ 0.0f, 0.2f, -8.5f }, 10.5f)
        .AddPositionKey({ 0.0f, 0.2f, -8.5f }, 25.7f)
        .AddPositionKey({ 0.0f, 0.2f, -15.0f }, 25.7f)
        .AddPositionKey({ 0.0f, 0.2f, -15.0f }, 39.7f)
        .AddPositionKey({ 1.5f, 1.5f, -0.8f }, 40.2f)
        .AddPositionKey({ 1.5f, 0.2f, -1.8f }, 41.5f)
        .AddPositionKey({ 1.5f, 1.0f, -2.4f }, 42.0f)
        .AddPositionKey({ 1.5f, 0.2f, -3.0f }, 42.5f)
        .AddPositionKey({ 1.5f, 0.4f, -3.6f }, 43.0f)
        .AddPositionKey({ 1.5f, 0.2f, -4.0f }, 43.5f)
        .AddPositionKey({ 1.5f, 0.4f, -4.6f }, 44.0f)
        .AddPositionKey({ 1.5f, 0.2f, -5.2f }, 44.5f)
        .AddPositionKey({ 1.5f, 0.2f, -5.8f }, 45.0f)
        .AddScaleKey({ 1.0f, 1.0f, 1.0f }, 0.0f)
        .AddScaleKey({ 1.0f, 1.0f, 1.0f }, 1.1f)
        .AddScaleKey({ 1.0f, 0.75f, 1.25f }, 1.2f)
        .AddScaleKey({ 1.0f, 0.5f, 0.5f }, 1.3f)
        .AddScaleKey({ 1.0f, 0.75f, 1.25f }, 1.4f)
        .AddScaleKey({ 1.0f, 1.0f, 1.0f }, 1.6f)
        .AddScaleKey({ 1.0f, 1.0f, 1.0f }, 1.8f)
        .AddScaleKey({ 1.0f, 0.75f, 1.25f }, 1.9f)
        .AddScaleKey({ 1.0f, 0.5f, 0.5f }, 2.1f)
        .AddScaleKey({ 1.0f, 0.75f, 1.25f }, 2.2f)
        .AddScaleKey({ 1.0f, 1.0f, 1.0f }, 2.3f)
        .AddRotationKey(Quaternion::Identity, 0.0f)
        .AddRotationKey(Quaternion::CreateFromAxisAngle(Math::Vector3::XAxis, 90.0f * Math::Constants::DegToRad), 0.1f)
        .AddRotationKey(Quaternion::CreateFromAxisAngle(Math::Vector3::XAxis, 180.0f * Math::Constants::DegToRad), 0.4f)
        .AddRotationKey(Quaternion::CreateFromAxisAngle(Math::Vector3::XAxis, 360.0f * Math::Constants::DegToRad), 0.8f)
        .AddRotationKey(Quaternion::CreateFromAxisAngle(Math::Vector3::XAxis, 90.0f * Math::Constants::DegToRad), 1.0f)
        .AddRotationKey(Quaternion::CreateFromAxisAngle(Math::Vector3::XAxis, 180.0f * Math::Constants::DegToRad), 1.2f)
        .AddRotationKey(Quaternion::CreateFromAxisAngle(Math::Vector3::XAxis, 270.0f * Math::Constants::DegToRad), 1.6f)
        .AddRotationKey(Quaternion::CreateFromAxisAngle(Math::Vector3::XAxis, 180.0f * Math::Constants::DegToRad), 2.0f)
        .AddRotationKey(Quaternion::CreateFromAxisAngle(Math::Vector3::XAxis, 90.0f * Math::Constants::DegToRad), 2.4f)
        .AddRotationKey(Quaternion::CreateFromAxisAngle(Math::Vector3::XAxis, 360.0f * Math::Constants::DegToRad), 2.8f)
        .AddRotationKey(Quaternion::CreateFromAxisAngle(Math::Vector3::XAxis, 270.0f * Math::Constants::DegToRad), 3.2f)
        .AddRotationKey(Quaternion::CreateFromAxisAngle(Math::Vector3::XAxis, 180.0f * Math::Constants::DegToRad), 3.6f)
        .AddRotationKey(Quaternion::CreateFromAxisAngle(Math::Vector3::XAxis, 90.0f * Math::Constants::DegToRad), 4.0f)
        .AddRotationKey(Quaternion::CreateFromAxisAngle(Math::Vector3::XAxis, 360.0f * Math::Constants::DegToRad), 4.5f)
        .AddRotationKey(Quaternion::CreateFromAxisAngle(Math::Vector3::XAxis, 360.0f * Math::Constants::DegToRad), 40.2f)
        .AddRotationKey(Quaternion::CreateFromAxisAngle(Math::Vector3::XAxis, 360.0f * Math::Constants::DegToRad), 40.5f)
        .AddRotationKey(Quaternion::CreateFromAxisAngle(Math::Vector3::XAxis, 270.0f * Math::Constants::DegToRad), 40.7f)
        .AddRotationKey(Quaternion::CreateFromAxisAngle(Math::Vector3::XAxis, 180.0f * Math::Constants::DegToRad), 40.9f)
        .AddRotationKey(Quaternion::CreateFromAxisAngle(Math::Vector3::XAxis, 90.0f * Math::Constants::DegToRad), 41.1f)
        .AddRotationKey(Quaternion::CreateFromAxisAngle(Math::Vector3::XAxis, 360.0f * Math::Constants::DegToRad), 41.3f)
        .AddRotationKey(Quaternion::CreateFromAxisAngle(Math::Vector3::XAxis, 270.0f * Math::Constants::DegToRad), 41.5f)
        .AddRotationKey(Quaternion::CreateFromAxisAngle(Math::Vector3::XAxis, 180.0f * Math::Constants::DegToRad), 41.7f)
        .AddRotationKey(Quaternion::CreateFromAxisAngle(Math::Vector3::XAxis, 90.0f * Math::Constants::DegToRad), 42.1f)
        .AddRotationKey(Quaternion::CreateFromAxisAngle(Math::Vector3::XAxis, 360.0f * Math::Constants::DegToRad), 42.3f)
        .AddRotationKey(Quaternion::CreateFromAxisAngle(Math::Vector3::XAxis, 270.0f * Math::Constants::DegToRad), 42.5f)
        .AddRotationKey(Quaternion::CreateFromAxisAngle(Math::Vector3::XAxis, 180.0f * Math::Constants::DegToRad), 42.7f)
        .AddRotationKey(Quaternion::CreateFromAxisAngle(Math::Vector3::XAxis, 90.0f * Math::Constants::DegToRad), 43.1f)
        .AddRotationKey(Quaternion::CreateFromAxisAngle(Math::Vector3::XAxis, 360.0f * Math::Constants::DegToRad), 43.3f)
        .AddRotationKey(Quaternion::CreateFromAxisAngle(Math::Vector3::XAxis, 270.0f * Math::Constants::DegToRad), 43.5f)
        .AddRotationKey(Quaternion::CreateFromAxisAngle(Math::Vector3::XAxis, 180.0f * Math::Constants::DegToRad), 43.7f)
        .AddRotationKey(Quaternion::CreateFromAxisAngle(Math::Vector3::XAxis, 90.0f * Math::Constants::DegToRad), 44.1f)
        .AddRotationKey(Quaternion::CreateFromAxisAngle(Math::Vector3::XAxis, 360.0f * Math::Constants::DegToRad), 44.3f)
        .AddRotationKey(Quaternion::CreateFromAxisAngle(Math::Vector3::XAxis, 270.0f * Math::Constants::DegToRad), 44.5f)
        .AddRotationKey(Quaternion::CreateFromAxisAngle(Math::Vector3::XAxis, 180.0f * Math::Constants::DegToRad), 44.7f)
        .AddRotationKey(Quaternion::CreateFromAxisAngle(Math::Vector3::XAxis, 90.0f * Math::Constants::DegToRad), 45.0f)
        .AddEventKey(std::bind(&GameState::EndOfAnimation, this), mTotalTimeAnimation)
        .Build();
}

void GameState::LoadAnimationForXBot()
{
    mAnimationXBot = AnimationBuilder()
        .AddPositionKey({ 0.0f, 0.0f, 0.0f }, 0.0f)
        .AddPositionKey({ 0.0f, 0.0f, 0.0f }, 1.9f)
        .AddPositionKey({ 0.5f, 0.0f, -2.5f }, 2.0f)
        .AddPositionKey({ 0.5f, 0.0f, -2.5f }, 23.5f)
        .AddPositionKey({ -3.0f, 0.0f, -0.8f }, 23.5f)
        .AddPositionKey({ -3.0f, 0.0f, -0.8f }, 40.2f)
        .AddPositionKey({ 1.5f, 0.0f, -0.8f }, 40.2f)
        .AddPositionKey({ 1.5f, 0.0f, 25.0f }, 40.7f)
        .AddRotationKey(Quaternion::Identity, 0.0f)
        .AddRotationKey(Quaternion::Identity, 1.9f)
        .AddRotationKey(Quaternion::CreateFromAxisAngle(Math::Vector3::YAxis, 180.0f * Math::Constants::DegToRad), 2.0f)
        .AddRotationKey(Quaternion::CreateFromAxisAngle(Math::Vector3::YAxis, 180.0f * Math::Constants::DegToRad), 22.5f)
        .AddRotationKey(Quaternion::CreateFromAxisAngle(Math::Vector3::YAxis, 90.0f * Math::Constants::DegToRad), 22.6f)
        .AddRotationKey(Quaternion::CreateFromAxisAngle(Math::Vector3::YAxis, 90.0f * Math::Constants::DegToRad), 38.2f)
        .AddRotationKey(Quaternion::CreateFromAxisAngle(Math::Vector3::YAxis, -90.0f * Math::Constants::DegToRad), 38.2f)
        .AddRotationKey(Quaternion::CreateFromAxisAngle(Math::Vector3::YAxis, -90.0f * Math::Constants::DegToRad), 40.2f)
        .AddRotationKey(Quaternion::CreateFromAxisAngle(Math::Vector3::YAxis, 180.0f * Math::Constants::DegToRad), 40.2f)
        .AddEventKey(std::bind(&GameState::StartAnimationXBotEvent, this), 0.0001f)
        .AddEventKey(std::bind(&GameState::OnMoveEvent, this), 1.6f)
        .AddEventKey(std::bind(&GameState::XBotFallingEvent, this), 2.0f)
        .AddEventKey(std::bind(&GameState::XBotBlood, this), 4.4f)
        .AddEventKey(std::bind(&GameState::XBotTurnOffBlood, this), 5.0f)
        .AddEventKey(std::bind(&GameState::XBotStandsUp, this), 16.3f)
        .AddEventKey(std::bind(&GameState::XBotTPose, this), 22.4f)
        .AddEventKey(std::bind(&GameState::XBotFlipKick, this), 24.5f)
        .AddEventKey(std::bind(&GameState::XBotVictoryDance, this), 34.2f)
        .AddEventKey(std::bind(&GameState::XBotWalking, this), 38.2f)
        .AddEventKey(std::bind(&GameState::XBotFallingEvent, this), 40.2f)
        .AddEventKey(std::bind(&GameState::EndOfAnimation, this), mTotalTimeAnimation)
        .Build();
}

void GameState::LoadAnimationForYBot()
{
    mAnimationYBot = AnimationBuilder()
        .AddPositionKey({ -5.0f, 0.0f, -1.0f }, 0.0f)
        .AddPositionKey({ -5.0f, 0.0f, -1.0f }, 25.3f)
        .AddPositionKey({ -25.0f, 12.0f, -1.0f }, 25.5f)
        .AddPositionKey({ -25.0f, 12.0f, -1.0f }, 27.3f)
        .AddPositionKey({ -150.0f, 150.0f, 0.0f }, 27.5f)
        .AddPositionKey(mSun.transform.position, 34.0f)
        .AddRotationKey(Quaternion::CreateFromAxisAngle(Math::Vector3::YAxis, -90.0f * Math::Constants::DegToRad), 0.0f)
        .AddEventKey(std::bind(&GameState::StartAnimationYBotEvent, this), 0.0001f)
        .AddEventKey(std::bind(&GameState::YBotLaughingSitting, this), 7.4f)
        .AddEventKey(std::bind(&GameState::EndOfAnimation, this), mTotalTimeAnimation)
        .Build();

    mAnimationYBotIndicator = AnimationBuilder()
        .AddPositionKey({ 0.0f, -10.0f, 0.0f }, 27.3f)
        .AddPositionKey({ -25.0f, 12.0f, -1.0f }, 27.3f)
        .AddPositionKey({ -150.0f, 150.0f, 0.0f }, 27.5f)
        .AddPositionKey(mSun.transform.position, 34.0f)
        .Build();
}

void GameState::LoadAnimationForCamera()
{
    mCameraAnimationPosition = AnimationBuilder()
        .AddPositionKey({ -6.0f, 2.0f, -6.0f }, 0.0001f)
        .AddPositionKey({ -6.0f, 2.0f, -6.0f }, 3.4f)
        .AddPositionKey({ 0.5f, 1.0f, -2.5f }, 3.8f)
        .AddPositionKey({ 0.5f, 1.0f, -2.5f }, 7.3f)
        .AddPositionKey({ -3.5f, 1.2f, -1.0f }, 7.5f)
        .AddPositionKey({ -3.5f, 1.2f, -1.0f }, 14.3f)
        .AddPositionKey({ 0.5f, 1.0f, -2.5f }, 14.5f)
        .AddPositionKey({ 0.5f, 1.0f, -2.5f }, 16.0f)
        .AddPositionKey({ 0.5f, 2.5f, -1.0f }, 17.0f)
        .AddPositionKey({ 0.5f, 2.5f, -1.0f }, 18.0f)
        .AddPositionKey({ 1.3f, 1.6f, -2.1f }, 19.0f)
        .AddPositionKey({ 1.3f, 1.6f, -2.1f }, 22.4f)
        .AddPositionKey({ -3.5f, 1.5f, -4.0f }, 22.4f)
        .AddPositionKey({ -3.5f, 1.5f, -4.0f }, 30.0f)
        .AddPositionKey({ -300.0f, 300.0f, -200.0f }, 30.0f)
        .AddPositionKey({ -300.0f, 300.0f, -200.0f }, 34.0f)
        .AddPositionKey({ -10.0f, 2.0f, -1.0f }, 34.0f)
        .AddEventKey(std::bind(&GameState::EndOfAnimation, this), mTotalTimeAnimation)
        .Build();

    mCameraAnimationLookAt = AnimationBuilder()
        .AddPositionKey({ -1.5f, 1.0f, 0.0f }, 0.0001f)
        .AddPositionKey({ -1.5f, 1.0f, 0.0f }, 3.4f)
        .AddPositionKey({ 40.0f, -40.0f, -40.0f }, 3.8f)
        .AddPositionKey({ 40.0f, -40.0f, -40.0f }, 7.3f)
        .AddPositionKey({ -40.0f, 0.0f, 0.0f }, 7.5f)
        .AddPositionKey({ -40.0f, 0.0f, 0.0f }, 14.3f)
        .AddPositionKey({ 40.0f, -40.0f, -40.0f }, 14.5f)
        .AddPositionKey({ 40.0f, -40.0f, -40.0f }, 16.0f)
        .AddPositionKey({ 30.0f, -40.0f, -70.0f }, 17.0f)
        .AddPositionKey({ 30.0f, -40.0f, -70.0f }, 18.0f)
        .AddPositionKey({ -40.0f, 0.0f, 0.0f }, 19.0f)
        .AddPositionKey({ -40.0f, 0.0f, 0.0f }, 22.4f)
        .AddPositionKey({ 0.0f, 0.0f, 40.0f }, 22.4f)
        .AddPositionKey({ 0.0f, 0.0f, 40.0f }, 27.3f)
        .AddPositionKey( mSun.transform.position , 27.3f)
        .AddPositionKey( mSun.transform.position , 27.3f)
        .AddPositionKey( mSun.transform.position , 30.0f)
        .AddPositionKey( mSun.transform.position , 34.0f)
        .AddPositionKey({ 20.0f, 0.0f, 0.0f }, 34.0f)
        .AddEventKey(std::bind(&GameState::EndOfAnimation, this), mTotalTimeAnimation)
        .Build();
}

void GameState::Terminate()
{
    mSkySphere.Terminate();
    mGround.Terminate();
    mYBotIndicator.Terminate();
    mSun.Terminate();
    mBall.Terminate();
    mYBot.Terminate();
    mXBot.Terminate();
    mParticleSystemEffect.Terminate();
    mParticleSystem.Terminate();
    mStandardEffect.Terminate();
    mGameWorld.Termiante();
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
        mAnimationTime += deltaTime;
        if (mBallAnimation.GetDuration() > 0.0f)
        {
            mBallAnimation.PlayEvents(prevTime, mAnimationTime);
            while (mAnimationTime > mBallAnimation.GetDuration())
            {
                mAnimationTime -= mBallAnimation.GetDuration();
            }
        }

        if (mAnimationXBot.GetDuration() > 0.0f)
        {
            mAnimationXBot.PlayEvents(prevTime, mAnimationTime);
            while (mAnimationTime > mAnimationXBot.GetDuration())
            {
                mAnimationTime -= mAnimationXBot.GetDuration();
            }
        }

        if (mAnimationYBot.GetDuration() > 0.0f)
        {
            mAnimationYBot.PlayEvents(prevTime, mAnimationTime);
            while (mAnimationTime > mAnimationYBot.GetDuration())
            {
                mAnimationTime -= mAnimationYBot.GetDuration();
            }
        }

        if (mTurnParticles)
        {
            mParticleSystem.Update(deltaTime);
        }

        mXBotAnimator.Update(deltaTime);
        mYBotAnimator.Update(deltaTime);

        mBall.transform = mBallAnimation.GetTransform(mAnimationTime);

        mXBot.transform = mAnimationXBot.GetTransform(mAnimationTime);
        mXBot.transform.position += mOffset;

        mYBot.transform = mAnimationYBot.GetTransform(mAnimationTime);
        mYBotIndicator.transform = mAnimationYBotIndicator.GetTransform(mAnimationTime);

        mCamera.SetPosition(mCameraAnimationPosition.GetTransform(mAnimationTime).position);
        mCamera.SetLookAt(mCameraAnimationLookAt.GetTransform(mAnimationTime).position);
        mGameWorld.Update(deltaTime);
    }

    if (input->IsKeyPressed(KeyCode::P))
    {
        PauseAnimation();
    }
    if (input->IsKeyPressed(KeyCode::ENTER))
    {
        UnpauseAnimation();
    }
    if (input->IsKeyPressed(KeyCode::R))
    {
        ResetAnimation();
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


void GameState::XBotTPose()
{
    mXBotAnimator.PlayAnimation(-1, false);
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
    mTurnParticles = true;
    ParticleSystemInfo info;
    info.textureId = TextureCache::Get()->LoadTexture("sprites/explosion.png");
    info.maxParticles = 100;
    info.particlePerEmit = { 10 , 10 };
    info.delay = 0.2f;
    info.lifeTime = FLT_MAX;
    info.timeBetweenEmit = { 0.2f, 0.4f };
    info.spawnAngle = { -30.0f, 30.0f };
    info.spawnSpeed = { 1.0f, 3.0f };
    info.spawnLifeTime = { 2.0f, 4.0f };
    info.spawnDirection = Math::Vector3::YAxis;
    info.spawnPosition = { 2.5f, 0.0f, -4.0f };
    info.startScale = { {0.3f, 0.5f, 0.3f}, {0.3f, 0.5f, 0.3f} };
    info.endScale = { {0.3f, 0.3f, 0.3f}, {0.3f, 0.3f, 0.3f} };
    info.startColor = { Colors::OrangeRed, Colors::OrangeRed };
    info.endColor = { Colors::OrangeRed, Colors::OrangeRed };
    mParticleSystem.Initialize(info);
}

void GameState::XBotTurnOffBlood()
{
    mTurnParticles = false;
}

void GameState::XBotStandsUp()
{
    mXBotAnimator.PlayAnimation(2, false);
}

void GameState::XBotFlipKick()
{
    mXBotAnimator.PlayAnimation(3, false);
}

void GameState::XBotWalking()
{
    mXBotAnimator.PlayAnimation(4, false);
}

void GameState::XBotVictoryDance()
{
    mXBotAnimator.PlayAnimation(5, false);
}

void GameState::StartAnimationYBotEvent()
{
    mYBotAnimator.PlayAnimation(1, true);
}

void GameState::YBotLaughingSitting()
{
    mYBotAnimator.PlayAnimation(1, true);
}

void GameState::EndOfAnimation()
{
    mEndOfAnimation = true;
}

void GameState::PauseAnimation()
{
    mPauseAnimation = true;
}

void GameState::UnpauseAnimation()
{
    mPauseAnimation = false;
}

void GameState::ResetAnimation()
{
    mAnimationTime = 0.0f;
    mOffset -= mOffset;
    mParticleSystem.Terminate();
    mEndOfAnimation = false;
    mPauseAnimation = false;
}

void GameState::Render()
{
    mStandardEffect.Begin();
        mStandardEffect.Render(mXBot);
        mStandardEffect.Render(mYBot);
        mStandardEffect.Render(mGround);
        mStandardEffect.Render(mBall);
        mStandardEffect.Render(mSun);
        mStandardEffect.Render(mYBotIndicator);
        mStandardEffect.Render(mSkySphere);
    mStandardEffect.End();
    mParticleSystemEffect.Begin();
        mParticleSystem.Render(mParticleSystemEffect);
    mParticleSystemEffect.End();
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