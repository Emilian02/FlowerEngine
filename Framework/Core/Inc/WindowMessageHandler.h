#pragma once

namespace FlowerEngine::Core
{
    class WindowMessageHandler
    {
    public:
        using CallBack = LRESULT(CALLBACK*)(HWND, UINT, WPARAM, LPARAM);

        void Hook(HWND window, CallBack cb);
        void Unhook();

        LRESULT ForwardMessage(HWND window, UINT message, WPARAM wParam, LPARAM lParam);

    private:
        HWND mWindow = nullptr;
        CallBack mPreviousCallback = nullptr;
    };
}