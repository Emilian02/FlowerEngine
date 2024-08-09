#pragma once

#include <FlowerEngine/Inc/FlowerEngine.h>

class ShapeState : public FlowerEngine::AppState
{
public: 
    void Initialize() override;
    void Terminate() override;
    void Update(float deltaTime) override;
    void Render() override;

protected:
    virtual void CreateShape();

    struct Vertex
    {
        FlowerEngine::Math::Vector3 position;
        FlowerEngine::Color color;
    };

    using Vertices = std::vector<Vertex>;
    Vertices mVertices;

    FlowerEngine::Graphics::Camera mCamera;
    FlowerEngine::Graphics::ConstantBuffer mConstantBuffer;
    FlowerEngine::Graphics::MeshBuffer mMeshBuffer;
    FlowerEngine::Graphics::VertexShader mVertexShader;
    FlowerEngine::Graphics::PixelShader mPixelShader;

};