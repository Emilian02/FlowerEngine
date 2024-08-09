#include "ShapeState.h"

using namespace FlowerEngine;
using namespace FlowerEngine::Math;
using namespace FlowerEngine::Graphics;
using namespace FlowerEngine::Core;
using namespace FlowerEngine::Input;


void ShapeState::CreateShape()
{
    // Vertices for each face (6 for each)

    // front
    mVertices.push_back({ { -0.5f, -0.5f, -0.5f }, Colors::Red });
    mVertices.push_back({ { -0.5f, 0.5f, -0.5f }, Colors::Green });
    mVertices.push_back({ { 0.5f, 0.5f, -0.5f }, Colors::Blue });

    mVertices.push_back({ { -0.5f, -0.5f, -0.5f }, Colors::Red });
    mVertices.push_back({ { 0.5f, 0.5f, -0.5f }, Colors::Blue });
    mVertices.push_back({ { 0.5f, -0.5f, -0.5f }, Colors::Green });

    // back
    mVertices.push_back({ { -0.5f, -0.5f, 0.5f }, Colors::Red });
    mVertices.push_back({ { 0.5f, 0.5f, 0.5f }, Colors::Blue });
    mVertices.push_back({ { -0.5f, 0.5f, 0.5f }, Colors::Green });

    mVertices.push_back({ { -0.5f, -0.5f, 0.5f }, Colors::Red });
    mVertices.push_back({ { 0.5f, -0.5f, 0.5f }, Colors::Green });
    mVertices.push_back({ { 0.5f, 0.5f, 0.5f }, Colors::Blue });

    // right
    mVertices.push_back({ { 0.5f, -0.5f, -0.5f }, Colors::Red });
    mVertices.push_back({ { 0.5f, 0.5f, -0.5f }, Colors::Blue });
    mVertices.push_back({ { 0.5f, 0.5f, 0.5f }, Colors::Green });

    mVertices.push_back({ { 0.5f, -0.5f, -0.5f }, Colors::Red });
    mVertices.push_back({ { 0.5f, 0.5f, 0.5f }, Colors::Green });
    mVertices.push_back({ { 0.5f, -0.5f, 0.5f }, Colors::Blue });

    // left
    mVertices.push_back({ { -0.5f, -0.5f, -0.5f }, Colors::Red });
    mVertices.push_back({ { -0.5f, 0.5f, 0.5f }, Colors::Green });
    mVertices.push_back({ { -0.5f, 0.5f, -0.5f }, Colors::Blue });

    mVertices.push_back({ { -0.5f, -0.5f, -0.5f }, Colors::Red });
    mVertices.push_back({ { -0.5f, -0.5f, 0.5f }, Colors::Blue });
    mVertices.push_back({ { -0.5f, 0.5f, 0.5f }, Colors::Green });

    // top
    mVertices.push_back({ { -0.5f, 0.5f, -0.5f }, Colors::Red });
    mVertices.push_back({ { -0.5f, 0.5f, 0.5f }, Colors::Green });
    mVertices.push_back({ { 0.5f, 0.5f, 0.5f }, Colors::Blue });

    mVertices.push_back({ { -0.5f, 0.5f, -0.5f }, Colors::Red });
    mVertices.push_back({ { 0.5f, 0.5f, 0.5f }, Colors::Blue });
    mVertices.push_back({ { 0.5f, 0.5f, -0.5f }, Colors::Green });

    // bottom
    mVertices.push_back({ { -0.5f, -0.5f, -0.5f }, Colors::Red });
    mVertices.push_back({ { 0.5f, -0.5f, 0.5f }, Colors::Blue });
    mVertices.push_back({ { -0.5f, -0.5f, 0.5f }, Colors::Green });

    mVertices.push_back({ { -0.5f, -0.5f, -0.5f }, Colors::Red });
    mVertices.push_back({ { 0.5f, -0.5f, -0.5f }, Colors::Green });
    mVertices.push_back({ { 0.5f, -0.5f, 0.5f }, Colors::Blue });
}

void ShapeState::Initialize()
{
    // create a simple shape in NDC space (-1/1, -1/1, 0/1)
    CreateShape();

    mCamera.SetPosition({ 0.0f, 1.0f, -3.0f });
    mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });

    mMeshBuffer.Initialize(mVertices.data(), sizeof(Vertex), mVertices.size());

    mConstantBuffer.Intialize(sizeof(Matrix4));

    std::filesystem::path shaderFile = L"../../Assets/Shaders/DoTransform.fx";
    mVertexShader.Initialize(shaderFile, VE_Position | VE_Color);
    mPixelShader.Initialize(shaderFile);
}

void ShapeState::Terminate()
{
    mPixelShader.Terminate();
    mVertexShader.Terminate();
    mConstantBuffer.Terminate();
    mMeshBuffer.Terminate();
    mVertices.clear();
}

float gRotationY = 0.0f;
float gRotationX = 0.0f;
void ShapeState::Update(float deltaTime)
{
    gRotationY += Math::Constants::HalfPi * deltaTime * 0.5f;
    gRotationX += Math::Constants::HalfPi * deltaTime * 0.25f;
}

void ShapeState::Render()
{
    mVertexShader.Bind();
    mPixelShader.Bind();

    // constant buffer

    Matrix4 matWorld = Matrix4::RotationY(gRotationY) * Matrix4::RotationX(gRotationX);
    Matrix4 matView = mCamera.GetViewMatrix();
    Matrix4 matProj = mCamera.GetProjectionMatrix();
    Matrix4 matFinal = matWorld * matView * matProj;
    Matrix4 wvp = Transpose(matFinal);
    mConstantBuffer.Update(&wvp);
    mConstantBuffer.BindVS(0);
    mMeshBuffer.Render();
}