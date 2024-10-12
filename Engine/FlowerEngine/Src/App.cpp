#include "Precompiled.h"
#include "App.h"
#include "AppState.h"

using namespace FlowerEngine;
using namespace FlowerEngine::Core;
using namespace FlowerEngine::Graphics;
using namespace FlowerEngine::Input;

void App::Run(const AppConfig& config)
{
    Window myWindow;
    myWindow.Initialize(
        GetModuleHandle(nullptr),
        config.appName,
        config.winWidth,
        config.winHeight
    );
    ASSERT(myWindow.IsActive(), "App: failed to create window");

    //init singeltons
    auto handle = myWindow.GetWindowHandle();
    GraphicsSystem::StaticInitialize(handle, false);
    InputSystem::StaticInitialize(handle);
    DebugUI::StaticInitialize(handle, false, true);
    SimpleDraw::StaticIntialize(config.maxDrawLines);
    TextureCache::StaticInitialize("../../Assets/Images/");

    //start state
    ASSERT(mCurrentState != nullptr, "App: no current state available");
    mCurrentState->Initialize();

    GraphicsSystem* gs = GraphicsSystem::Get();
    InputSystem* input = InputSystem::Get();
    //run program
    mRunning = true;
    while (mRunning)
    {
        // update
        myWindow.ProcessMessage();
        input->Update();

        if (!myWindow.IsActive() || input->IsKeyPressed(KeyCode::ESCAPE))
        {
            Quit();
        }

        if (mNextState != nullptr)
        {
            mCurrentState->Terminate();
            mCurrentState = std::exchange(mNextState, nullptr);
            mCurrentState->Initialize();
        }

        float deltaTime = TimeUtil::GetDeltaTime();
#ifdef _DEBUG
        if (deltaTime < 0.5f)
#endif
        {
            mCurrentState->Update(deltaTime);
        }

        gs->BeginRender();
            mCurrentState->Render();
            DebugUI::BeginRender();
                mCurrentState->DebugUI();
            DebugUI::EndRender();
        gs->EndRender();
    }
    //end state
    mCurrentState->Terminate();

    //terminate singletons
    TextureCache::StaticTerminate();
    SimpleDraw::StaticTerminate();
    DebugUI::StaticTerminate();
    InputSystem::StaticTerminate();
    GraphicsSystem::StaticTerminate();

    myWindow.Terminate();
}

void App::Quit()
{
    mRunning = false;
}

void App::ChangeState(const std::string& stateName)
{
    auto iter = mAppStates.find(stateName);
    if (iter != mAppStates.end())
    {
        mNextState = iter->second.get();
    }
}
