#include <FlowerEngine/Inc/FlowerEngine.h>

#include "GameState.h"

using namespace FlowerEngine;

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
    AppConfig config;
    config.appName = L"Hello Mesh Builder";

    App& myApp = MainApp();
    myApp.AddState<CubeMesh>("CubeMesh");
    myApp.AddState<RectMesh>("RectMesh");
    myApp.AddState<PlaneMesh>("PlaneMesh");
    myApp.AddState<CylinderMesh>("CylinderMesh");
    myApp.AddState<SphereMesh>("SphereMesh");
    myApp.AddState<SkySphereMesh>("SkySphereMesh");
    myApp.AddState<SkyBoxMesh>("SkyBoxMesh");
    myApp.Run(config);

    return (0);
}