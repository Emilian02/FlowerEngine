#pragma once

#include <FlowerEngine/Inc/FlowerEngine.h>

enum class CharacterDraw
{
    None,
    Character01,
    Character02
};

class GameState : public FlowerEngine::AppState
{
public: 
    void Initialize() override;
    void Terminate() override;
    void Update(float deltaTime) override;
    void Render() override;
    void DebugUI() override;

protected:
    void UpdateCamera(float deltaTime);

    FlowerEngine::Graphics::Camera mCamera;
    FlowerEngine::Graphics::Camera mRenderTargetCamera;

    FlowerEngine::Graphics::RenderGroup mCharacter01;
    FlowerEngine::Graphics::RenderGroup mCharacter02;
    FlowerEngine::Graphics::DirectionalLight mDirectionalLight;
    FlowerEngine::Graphics::StandardEffect mStandardEffect;
    FlowerEngine::Graphics::StandardEffect mTargetStandardEffect;
    FlowerEngine::Graphics::RenderTarget mRenderTarget;

    CharacterDraw mCharacterDraw = CharacterDraw::None;

private:
    int characterIndex = 0;
};