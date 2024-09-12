#pragma once

#include <FlowerEngine/Inc/FlowerEngine.h>

enum class SolarSystemDraws
{
    Sun,
    Mercury,
    Venus,
    Earth,
    Mars,
    Jupiter,
    Saturn,
    Uranus,
    Neptune,
    Pluto
};

class SolarSystem
{
public:
    //Rendering
    FlowerEngine::Graphics::MeshBuffer mMeshBuffer;
    FlowerEngine::Graphics::Texture mDiffuseTexture;
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

    SolarSystem mSolarSystem[11];

    FlowerEngine::Graphics::Camera mCamera;
    FlowerEngine::Graphics::Camera mRenderTargetCamera;
    FlowerEngine::Graphics::ConstantBuffer mConstantBuffer;
    FlowerEngine::Graphics::MeshBuffer mMeshBuffer;
    FlowerEngine::Graphics::VertexShader mVertexShader;
    FlowerEngine::Graphics::PixelShader mPixelShader;
    FlowerEngine::Graphics::Texture mDiffuseTexture;
    FlowerEngine::Graphics::Sampler mSampler;
    FlowerEngine::Graphics::RenderTarget mRenderTarget;
};