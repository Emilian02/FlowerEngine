#include "ShapeState.h"

using namespace FlowerEngine;
using namespace FlowerEngine::Math;
using namespace FlowerEngine::Graphics;
using namespace FlowerEngine::Core;
using namespace FlowerEngine::Input;

void ShapeState::Initialize()
{
    CreateShape();

    auto device = GraphicsSystem::Get()->GetDevice();

    //==========================================================
    // create a vertex buffer
    D3D11_BUFFER_DESC bufferDesc{};
    bufferDesc.ByteWidth = static_cast<UINT>(mVertices.size() * sizeof(Vertex));
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bufferDesc.MiscFlags = 0;
    bufferDesc.StructureByteStride = 0;

    D3D11_SUBRESOURCE_DATA initData{};
    initData.pSysMem = mVertices.data();

    HRESULT hr = device->CreateBuffer(&bufferDesc, &initData, &mVertexBuffer);
    ASSERT(SUCCEEDED(hr), "Failed to create vertex buffer");

    //==========================================================
    // create a vertex shader
    std::filesystem::path shaderFile = L"../../Assets/Shaders/DoColor.fx";

    DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;
    ID3DBlob* shaderBlob = nullptr;
    ID3DBlob* errorBlob = nullptr;
    hr = D3DCompileFromFile(
        shaderFile.c_str(),
        nullptr,
        D3D_COMPILE_STANDARD_FILE_INCLUDE,
        "VS", "vs_5_0",
        shaderFlags, 0,
        &shaderBlob,
        &errorBlob);

    if (errorBlob != nullptr && errorBlob->GetBufferPointer() != nullptr)
    {
        LOG("%s", static_cast<const char*>(errorBlob->GetBufferPointer()));
    }

    ASSERT(SUCCEEDED(hr), "Failed to compiled vertex shader");

    hr = device->CreateVertexShader(
        shaderBlob->GetBufferPointer(),
        shaderBlob->GetBufferSize(),
        nullptr,
        &mVertexShader);
    
    ASSERT(SUCCEEDED(hr), "Failed to create vertex shader");

    //==========================================================
    // create input layout
    std::vector<D3D11_INPUT_ELEMENT_DESC> vertexLayout;
    vertexLayout.push_back({ "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT });
    vertexLayout.push_back({ "Color", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT });

    hr = device->CreateInputLayout(
        vertexLayout.data(),
        (UINT)vertexLayout.size(),
        shaderBlob->GetBufferPointer(),
        shaderBlob->GetBufferSize(),
        &mInputLayout);

    ASSERT(SUCCEEDED(hr), "Failed to create input layout");
    SafeRelease(shaderBlob);
    SafeRelease(errorBlob);
    //==========================================================
    // create a pixel shader

    hr = D3DCompileFromFile(
        shaderFile.c_str(),
        nullptr,
        D3D_COMPILE_STANDARD_FILE_INCLUDE,
        "PS", "ps_5_0",
        shaderFlags, 0,
        &shaderBlob,
        &errorBlob);

    if (errorBlob != nullptr && errorBlob->GetBufferPointer() != nullptr)
    {
        LOG("%s", static_cast<const char*>(errorBlob->GetBufferPointer()));
    }

    ASSERT(SUCCEEDED(hr), "Failed to compiled pixel shader");

    hr = device->CreatePixelShader(
        shaderBlob->GetBufferPointer(),
        shaderBlob->GetBufferSize(),
        nullptr,
        &mPixelShader);

    ASSERT(SUCCEEDED(hr), "Failed to create pixel shader");

    SafeRelease(shaderBlob);
    SafeRelease(errorBlob);
}

void ShapeState::Terminate()
{
    mVertices.clear();
    SafeRelease(mPixelShader);
    SafeRelease(mInputLayout);
    SafeRelease(mVertexShader);
    SafeRelease(mVertexBuffer);
}

void ShapeState::Render()
{
    auto context = GraphicsSystem::Get()->GetContext();

    context->VSSetShader(mVertexShader, nullptr, 0);
    context->IASetInputLayout(mInputLayout);
    context->PSSetShader(mPixelShader, nullptr, 0);

    context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    context->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
    context->Draw((UINT)mVertices.size(), 0);
}

// triangle shape
void TriangleShapeState::Update(float deltatime)
{
}

void TriangleShapeState::CreateShape()
{
    mVertices.push_back({ { -0.5f, 0.0f, 0.0f }, Colors::Red });
    mVertices.push_back({ { 0.0f, 0.75f, 0.0f }, Colors::Green });
    mVertices.push_back({ { 0.5f, 0.0f, 0.0f }, Colors::Blue });
}

// square shape
void SquareShapeState::Update(float deltatime)
{

}

void SquareShapeState::CreateShape()
{
    mVertices.push_back({ { -0.5f, -0.5f, 0.0f }, Colors::Red });
    mVertices.push_back({ { -0.5f, 0.5f, 0.0f }, Colors::Green });
    mVertices.push_back({ { 0.5f, 0.5f, 0.0f }, Colors::Blue });

    mVertices.push_back({ { -0.5f, -0.5f, 0.0f }, Colors::Red });
    mVertices.push_back({ { 0.5f, 0.5f, 0.0f }, Colors::Blue });
    mVertices.push_back({ { 0.5f, -0.5f, 0.0f }, Colors::Green });
}

// star shape
void StarShapeState::Update(float deltatime)
{
    if (InputSystem::Get()->IsKeyPressed(KeyCode::RIGHT))
    {
        MainApp().ChangeState("FishShapeState");
    }
}

void StarShapeState::CreateShape()
{
    mVertices.push_back({ {-0.25f, -0.5f, 0.0f}, Colors::Yellow });
    mVertices.push_back({ {-0.05f, 0.2f, 0.0f}, Colors::Yellow });
    mVertices.push_back({ {0.3f, 0.0f, 0.0f}, Colors::Yellow });

    mVertices.push_back({ { -0.3f, 0.0f, 0.0f }, Colors::Yellow });
    mVertices.push_back({ { 0.05f, 0.2f, 0.0f }, Colors::Yellow });
    mVertices.push_back({ { 0.25f, -0.5f, 0.0f }, Colors::Yellow });

    mVertices.push_back({ {-0.15f, -0.2f, 0.0f}, Colors::Yellow });
    mVertices.push_back({ {0.0f, 0.45f, 0.0f}, Colors::Yellow });
    mVertices.push_back({ {0.15f, -0.2f, 0.0f}, Colors::Yellow });
}

// fish shape
void FishShapeState::Update(float deltatime)
{    
    if (InputSystem::Get()->IsKeyPressed(KeyCode::RIGHT))
    {
        MainApp().ChangeState("DiamondShapeState");
    }
    else if (InputSystem::Get()->IsKeyPressed(KeyCode::LEFT))
    {
        MainApp().ChangeState("StarShapeState");
    }
}    
     
void FishShapeState::CreateShape()
{
    mVertices.push_back({ {-0.15f, 0.0f, 0.0f}, Colors::Purple });
    mVertices.push_back({ {0.02f, 0.1f, 0.0f}, Colors::Purple });
    mVertices.push_back({ {0.02f, -0.1f, 0.0f}, Colors::Purple });

    mVertices.push_back({ {-0.4f, 0.0f, 0.001f}, Colors::AliceBlue });
    mVertices.push_back({ {-0.2f, 0.2f, 0.001f}, Colors::Aqua });
    mVertices.push_back({ {-0.2f, -0.2f, 0.001f}, Colors::Aqua });

    mVertices.push_back({ {-0.4f, 0.0f, 0.001f}, Colors::MintCream });
    mVertices.push_back({ {0.1f, 0.5f, 0.001f}, Colors::LightCoral });
    mVertices.push_back({ {0.1f, -0.5f, 0.001f}, Colors::LightCoral });

    mVertices.push_back({ {0.05f, 0.0f, 0.001f}, Colors::Purple });
    mVertices.push_back({ {0.25f, 0.25f, 0.001f}, Colors::MediumPurple });
    mVertices.push_back({ {0.25f, -0.25f, 0.001f}, Colors::MediumPurple });
        
}

// Diamond shape
void DiamondShapeState::Update(float deltatime)
{    
    if (InputSystem::Get()->IsKeyPressed(KeyCode::LEFT))
    {
        MainApp().ChangeState("FishShapeState");
    }
}    
     
void DiamondShapeState::CreateShape()
{
    mVertices.push_back({ {-0.2f, 0.0f, 0.0f}, Colors::CadetBlue });
    mVertices.push_back({ {0.0f, 0.4f, 0.0f}, Colors::White });
    mVertices.push_back({ {0.0f, 0.0f, 0.0f}, Colors::CadetBlue });

    mVertices.push_back({ {0.0f, 0.4f, 0.0f}, Colors::White });
    mVertices.push_back({ {0.2f, 0.0f, 0.0f}, Colors::CadetBlue });
    mVertices.push_back({ {0.0f, 0.0f, 0.0f}, Colors::CadetBlue });

    mVertices.push_back({ {0.2f, 0.0f, 0.0f}, Colors::CadetBlue });
    mVertices.push_back({ {0.0f, -0.4f, 0.0f}, Colors::White });
    mVertices.push_back({ {0.0f, 0.0f, 0.0f}, Colors::CadetBlue });

    mVertices.push_back({ {-0.2f, 0.0f, 0.0f}, Colors::CadetBlue });
    mVertices.push_back({ {0.0f, 0.0f, 0.0f}, Colors::CadetBlue });
    mVertices.push_back({ {0.0f, -0.4f, 0.0f}, Colors::White });

}