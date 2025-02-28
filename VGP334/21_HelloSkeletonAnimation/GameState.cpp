 #include "GameState.h"

using namespace FlowerEngine;
using namespace FlowerEngine::Math;
using namespace FlowerEngine::Graphics;
using namespace FlowerEngine::Core;
using namespace FlowerEngine::Input;

void GameState::Initialize()
{
    mCamera.SetPosition({ 0.0f, 2.0f, -3.0f });
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

    mCharacter01.Initialize(L"../../Assets/Models/Character04/XBot.model", &mCharacterAnimator);
    ModelCache::Get()->AddAnimation(mCharacter01.modelId, L"../../Assets/Models/Character04/SillyDancing.animset");
    ModelCache::Get()->AddAnimation(mCharacter01.modelId, L"../../Assets/Models/Character04/ShovedReactionWithSpin.animset");
    ModelCache::Get()->AddAnimation(mCharacter01.modelId, L"../../Assets/Models/Character04/RunLookBack.animset");
    ModelCache::Get()->AddAnimation(mCharacter01.modelId, L"../../Assets/Models/Character04/RemovingDriver.animset");
    ModelCache::Get()->AddAnimation(mCharacter01.modelId, L"../../Assets/Models/Character04/SoccerTrip.animset");
    mCharacterAnimator.Initialize(mCharacter01.modelId);

    Mesh ground = MeshBuilder::CreateGroundPlane(10, 10, 1.0f);
    mGround.meshBuffer.Initialize(ground);
    mGround.diffuseMapId = TextureCache::Get()->LoadTexture("misc/concrete.jpg");
    mGround.material.ambient = { 0.3f, 0.3f, 0.3f, 1.0f };
    mGround.material.diffuse = { 0.8f, 0.8f, 0.8f, 1.0f };
    mGround.material.specular = { 0.9f, 0.9f, 0.9f, 1.0f };
    mGround.material.power = 20.f;
}

void GameState::Terminate()
{
    mGround.Terminate();
    mCharacter01.Terminate();
    mStandardEffect.Terminate();
}

void GameState::Update(float deltaTime)
{
    UpdateCamera(deltaTime);

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

void GameState::Render()
{
    mStandardEffect.Begin();
        if (mShowSkeleton)
        {
            AnimationUtil::BoneTransforms boneTranforms;
            AnimationUtil::ComputeBoneTransforms(mCharacter01.modelId, boneTranforms, &mCharacterAnimator);
            AnimationUtil::DrawSkeleton(mCharacter01.modelId, boneTranforms);
        }
        else
        {
            mStandardEffect.Render(mCharacter01);
        }
        mStandardEffect.Render(mGround);
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
    ImGui::Checkbox("ShowSkeleton", &mShowSkeleton);

    int maxAnimations = mCharacterAnimator.GetAnimationCount();
    if (ImGui::DragInt("AnimIndex", &mAnimationIndex, 1, -1, maxAnimations - 1))
    {
        mCharacterAnimator.PlayAnimation(mAnimationIndex, true);
    }

    ImGui::Separator();

    mStandardEffect.DebugUI();

    ImGui::End();

    SimpleDraw::Render(mCamera);
}