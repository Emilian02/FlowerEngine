#include "Precompiled.h"
#include "App.h"

using namespace FlowerEngine;
using namespace FlowerEngine::Core;

void App::Run()
{
    Window myWindow;
    myWindow.Initialize(
        GetModuleHandle(nullptr),
        L"HelloWindow",
        720,
        720
    );

    (void)TimeUtil::GetTime();

    mRunning = true;
    while (mRunning)
    {
        // update
        myWindow.ProcessMessage();

        if (!myWindow.IsActive())
        {
            Quit();
        }
    }

    myWindow.Terminate();
}

void App::Quit()
{
    mRunning = false;
}
