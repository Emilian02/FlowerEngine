#include <FlowerEngine/Inc/FlowerEngine.h>

#include "GameState.h"

using namespace FlowerEngine;

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
    AppConfig config;
    config.appName = L"Hello Puddles";

    App& myApp = MainApp();
    myApp.AddState<GameState>("GameState");
    myApp.Run(config);

    return (0);
}