#pragma once

namespace FlowerEngine
{
    class App final
    {
    public:
        void Run();
        void Quit();

    private:
        bool mRunning = false;

    };
}