#pragma once

#include <FlowerEngine/Inc/FlowerEngine.h>

class ShapeState : public FlowerEngine::AppState
{
public: 
    void Initialize() override;
    void Terminate() override;
    void Render() override;

protected:
    virtual void CreateShape() {}

    struct Vertex
    {
        FlowerEngine::Math::Vector3 position;
        FlowerEngine::Color color;
    };

    using Vertices = std::vector<Vertex>;
    Vertices mVertices;

    ID3D11Buffer* mVertexBuffer = nullptr;
    ID3D11VertexShader* mVertexShader = nullptr;
    ID3D11InputLayout* mInputLayout = nullptr;
    ID3D11PixelShader* mPixelShader = nullptr;
};

class TriangleShapeState : public ShapeState
{
public:
    void Update(float deltatime) override;

protected:
    void CreateShape() override;
};

class SquareShapeState : public ShapeState
{
public:
    void Update(float deltatime) override;

protected:
    void CreateShape() override;
};

class StarShapeState : public ShapeState
{
public:
    void Update(float deltatime) override;

protected:
    void CreateShape() override;
};

class FishShapeState : public ShapeState
{
public:
    void Update(float deltatime) override;

protected:
    void CreateShape() override;
};

class DiamondShapeState : public ShapeState
{
public:
    void Update(float deltatime) override;

protected:
    void CreateShape() override;
};