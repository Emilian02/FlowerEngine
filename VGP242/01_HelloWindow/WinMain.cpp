#include <FlowerEngine/Inc/FlowerEngine.h>


class GameState :public FlowerEngine::AppState
{
public:
    void Initialize() override
    {
        LOG("Game State Initialized");
    }

    void Terminate() override
    {
        LOG("Game State Terminated");
    }
};

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
    FlowerEngine::AppConfig config;
    config.appName = L"Hello Window";

    FlowerEngine::App& myApp = FlowerEngine::MainApp();
    myApp.AddState<GameState>("GameState");
    myApp.Run(config);
        
    return(0);
}