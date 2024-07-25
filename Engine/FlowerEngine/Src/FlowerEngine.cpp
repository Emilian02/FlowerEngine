#include "Precompiled.h"
#include "FlowerEngine.h"

FlowerEngine::App& FlowerEngine::MainApp()
{
    static FlowerEngine::App sApp;
    return sApp;
}