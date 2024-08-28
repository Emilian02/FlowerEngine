#pragma once

#include <FlowerEngine/Inc/FlowerEngine.h>

class GameState : public FlowerEngine::AppState
{
public: 
    void Initialize() override;
    void Terminate() override;
    void Update(float deltaTime) override;
    void Render() override;

protected:
    void UpdateCamera(float deltaTime);

    virtual void CreateMesh(){}

    FlowerEngine::Graphics::Camera mCamera;
    FlowerEngine::Graphics::ConstantBuffer mConstantBuffer;
    FlowerEngine::Graphics::MeshBuffer mMeshBuffer;
    FlowerEngine::Graphics::VertexShader mVertexShader;
    FlowerEngine::Graphics::PixelShader mPixelShader;
    FlowerEngine::Graphics::Texture mDiffuseTexture;
    FlowerEngine::Graphics::Sampler mSampler;
    std::filesystem::path shaderFile;
};

class CubeMesh : public GameState
{
public:
    void Update(float deltatime) override;

protected:
    void CreateMesh() override;
};

class RectMesh : public GameState
{
public:
    void Update(float deltatime) override;

protected:
    void CreateMesh() override;
};

class PlaneMesh : public GameState
{
public:
    void Update(float deltatime) override;

protected:
    void CreateMesh() override;
};

class CylinderMesh : public GameState
{
public:
    void Update(float deltatime) override;

protected:
    void CreateMesh() override;
};

class SphereMesh : public GameState
{
public:
    void Update(float deltatime) override;

protected:
    void CreateMesh() override;
};

class SkyBoxMesh : public GameState
{
public:
    void Update(float deltatime) override;

protected:
    void CreateMesh() override;
};

class SkySphereMesh : public GameState
{
public:
    void Update(float deltatime) override;

protected:
    void CreateMesh() override;
};