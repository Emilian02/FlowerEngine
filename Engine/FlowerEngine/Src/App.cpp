#include "Precompiled.h"
#include "App.h"
#include "AppState.h"

using namespace FlowerEngine;
using namespace FlowerEngine::Core;

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

    ASSERT(mCurrentState != nullptr, "App: no current state available");
    mCurrentState->Initialize();

    mRunning = true;
    while (mRunning)
    {
        // update
        myWindow.ProcessMessage();

        if (!myWindow.IsActive())
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

        //rendering
    }
    mCurrentState->Terminate();

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
