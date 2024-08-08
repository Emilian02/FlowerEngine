#include <FlowerEngine/Inc/FlowerEngine.h>

#include "ShapeState.h"

using namespace FlowerEngine;

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
    AppConfig config;
    config.appName = L"Hello Shapes";

    App& myApp = MainApp();
    myApp.AddState<StarShapeState>("StarShapeState");
    myApp.AddState<FishShapeState>("FishShapeState");
    myApp.AddState<DiamondShapeState>("DiamondShapeState");
    myApp.Run(config);

    return (0);
}