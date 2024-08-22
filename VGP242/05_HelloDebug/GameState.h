#pragma once

#include <FlowerEngine/Inc/FlowerEngine.h>

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
    FlowerEngine::Graphics::ConstantBuffer mConstantBuffer;
    FlowerEngine::Graphics::MeshBuffer mMeshBuffer;
    FlowerEngine::Graphics::VertexShader mVertexShader;
    FlowerEngine::Graphics::PixelShader mPixelShader;
    FlowerEngine::Graphics::Texture mDiffuseTexture;
    FlowerEngine::Graphics::Sampler mSampler;
};