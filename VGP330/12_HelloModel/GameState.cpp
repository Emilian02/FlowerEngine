 #include "GameState.h"

using namespace FlowerEngine;
using namespace FlowerEngine::Math;
using namespace FlowerEngine::Graphics;
using namespace FlowerEngine::Core;
using namespace FlowerEngine::Input;

const char* gCharacters[] =
{
    "None",
    "Character01",
    "Character02"
};

void GameState::Initialize()
{
    mCamera.SetPosition({ 0.0f, 2.0f, -3.0f });
    mCamera.SetLookAt({ 0.0f, 1.0f, 0.0f });
    mCamera.SetAspectRatio(0.0f);

    mRenderTargetCamera.SetPosition({ 0.0, 1.5f, -2.0f });
    mRenderTargetCamera.SetLookAt({ 0.0f, 1.2f, 0.0f });
    mRenderTargetCamera.SetAspectRatio(1.0f);

    mDirectionalLight.direction = Normalize({ 1.0f, -1.0f, 1.0f });
    mDirectionalLight.ambient = { 0.3f, 0.3f, 0.3f, 1.0f };
    mDirectionalLight.diffuse = { 0.7f, 0.7f, 0.7f, 1.0f };
    mDirectionalLight.specular = { 0.9f, 0.9f, 0.9f, 1.0f };

    Mesh mesh = MeshBuilder::CreateSphere(100, 100, 1.0f);

    std::filesystem::path shaderFile = L"../../Assets/Shaders/Standard.fx";
    mStandardEffect.Intitialize(shaderFile);
    mStandardEffect.SetCamera(mCamera);
    mStandardEffect.SetDirectionalLight(mDirectionalLight);

    mTargetStandardEffect.Intitialize(shaderFile);
    mTargetStandardEffect.SetCamera(mRenderTargetCamera);
    mTargetStandardEffect.SetDirectionalLight(mDirectionalLight);

    mCharacter01.Initialize(L"../../Assets/Models/Character02/YBot.model");
    mCharacter01.transform.position = { -1.0f, 0.0f, 0.0f };

    mCharacter02.Initialize(L"../../Assets/Models/Character04/XBot.model");
    mCharacter02.transform.position = { 1.0f, 0.0f, 0.0f };

    const uint32_t size = 512;
    mRenderTarget.Initialize(size, size, Texture::Format::RGBA_U8);
}

void GameState::Terminate()
{
    mRenderTarget.Terminate();
    mCharacter02.Terminate();
    mCharacter01.Terminate();
    mTargetStandardEffect.Terminate();
    mStandardEffect.Terminate();
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

void GameState::Render()
{
    mRenderTarget.BeginRender();
        mTargetStandardEffect.Begin();
            if (mCharacterDraw == CharacterDraw::Character01)
            {
                mRenderTargetCamera.SetPosition(mCharacter01.transform.position + Vector3{0.0, 1.2f, -2.0f});
                mTargetStandardEffect.Render(mCharacter01);
            }
            else if (mCharacterDraw == CharacterDraw::Character02)
            {
                mRenderTargetCamera.SetPosition(mCharacter02.transform.position + Vector3{ 0.0, 1.2f, -2.0f });
                mTargetStandardEffect.Render(mCharacter02);
            }
         mTargetStandardEffect.End();
    mRenderTarget.EndRender();

    mStandardEffect.Begin();
        mStandardEffect.Render(mCharacter01);
        mStandardEffect.Render(mCharacter02);
    mStandardEffect.End();

    SimpleDraw::AddGroundPlane(10.0f, Colors::White);
    SimpleDraw::Render(mCamera);
}

void GameState::DebugUI()
{
    ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    characterIndex = static_cast<int>(mCharacterDraw);

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

    ImGui::Separator();
    if (ImGui::CollapsingHeader("Character Selection", ImGuiTreeNodeFlags_DefaultOpen))
    {
        if (ImGui::Combo("Characters", &characterIndex, gCharacters, static_cast<int>(std::size(gCharacters))))
        {
            mCharacterDraw = (CharacterDraw)characterIndex;
        }
    }

    ImGui::Separator();
    ImGui::Text("RendeTarget");
    ImGui::Image(
        mRenderTarget.GetRawData(),
        { 128, 128 },
        { 0, 0 },
        { 1, 1 },
        { 1, 1, 1, 1},
        { 1, 1, 1, 1}
        );

    mStandardEffect.DebugUI();
    ImGui::End();
}