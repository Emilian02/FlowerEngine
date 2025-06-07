#include "EditTemplateState.h"
#include "CustomDebugDrawComponent.h"
#include "CustomDebugDrawService.h"

using namespace FlowerEngine;
using namespace FlowerEngine::Math;
using namespace FlowerEngine::Graphics;
using namespace FlowerEngine::Core;
using namespace FlowerEngine::Input;
using namespace FlowerEngine::Audio;

Service* CustomServiceMake(const std::string& serviceName, GameWorld& gameWorld)
{
    if (serviceName == "CustomDebugDrawService")
    {
        return gameWorld.AddService<CustomDebugDrawService>();
    }

    return nullptr;
}

Component* CustomComponentMake(const std::string& componentName, GameObject& gameObject)
{
    if (componentName == "CustomDebugDrawComponent")
    {
        return gameObject.AddComponent<CustomDebugDrawComponent>();
    }

    return nullptr;
}

Component* CustomComponentGet(const std::string& componentName, GameObject& gameObject)
{
    if (componentName == "CustomDebugDrawComponent")
    {
        return gameObject.GetComponent<CustomDebugDrawComponent>();
    }

    return nullptr;
}

void EditTemplateState::Initialize()
{
    // adds delegate callback to create a custom service
    GameWorld::SetCustomService(CustomServiceMake);
    GameObjectFactory::SetCustomMake(CustomComponentMake);
    GameObjectFactory::SetCustomGet(CustomComponentGet);

    mGameWorld.LoadLevel(L"../../Assets/Templates/Levels/test_level.json");
}

void EditTemplateState::Terminate()
{
    mGameWorld.Termiante();
}

void EditTemplateState::Update(float deltaTime)
{
    mGameWorld.Update(deltaTime);
}
void EditTemplateState::Render()
{
    mGameWorld.Render();
}

void EditTemplateState::DebugUI()
{
    ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    mGameWorld.DebugUI();
    ImGui::End();
}