 #include "GameState.h"

using namespace FlowerEngine;
using namespace FlowerEngine::Math;
using namespace FlowerEngine::Graphics;
using namespace FlowerEngine::Core;
using namespace FlowerEngine::Input;
using namespace FlowerEngine::Audio;

void GameState::Initialize()
{
    mGameWorld.AddService<CameraService>();
    mGameWorld.AddService<RenderService>();
    mGameWorld.Initialize();

    GameObject* transformGO = mGameWorld.CreateGameObject("Transform", L"../../Assets/Templates/mesh_obj.json");
    transformGO->Initialize();

    GameObject* cameraGO = mGameWorld.CreateGameObject("Camera", L"../../Assets/Templates/fps_camera.json");
    cameraGO->Initialize();
}

void GameState::Terminate()
{
    mGameWorld.Termiante();
}

void GameState::Update(float deltaTime)
{
    mGameWorld.Update(deltaTime);
}
void GameState::Render()
{
    mGameWorld.Render();
}

void GameState::DebugUI()
{
    ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    mGameWorld.DebugUI();
    ImGui::End();
}