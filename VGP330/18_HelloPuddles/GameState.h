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
    FlowerEngine::Graphics::DirectionalLight mDirectionalLight;

    FlowerEngine::Graphics::StandardEffect mStandardEffect;
    FlowerEngine::Graphics::PuddleEffect mPuddleEffect;

    FlowerEngine::Graphics::RenderGroup mCharacter01;
    FlowerEngine::Graphics::RenderGroup mCharacter02;

    FlowerEngine::Graphics::RenderObject mGround;
    FlowerEngine::Graphics::Mesh mPlain;

    CharacterDraw mCharacterDraw = CharacterDraw::None;

private:
    int characterIndex = 0;
};