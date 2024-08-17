#include "GameState.h"

using namespace FlowerEngine;
using namespace FlowerEngine::Math;
using namespace FlowerEngine::Graphics;
using namespace FlowerEngine::Core;
using namespace FlowerEngine::Input;



void GameState::Initialize()
{
    // create a simple shape in NDC space (-1/1, -1/1, 0/1)

    MeshPX mesh = MeshBuilder::CreateSpherePX(60, 60, 1.0f);

    mCamera.SetPosition({ 0.0f, 1.0f, -3.0f });
    mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });

    mMeshBuffer.Initialize<MeshPX>(mesh);

    mConstantBuffer.Intialize(sizeof(Matrix4));

    //std::filesystem::path shaderFile = L"../../Assets/Shaders/DoTransform.fx";
    std::filesystem::path shaderFile = L"../../Assets/Shaders/DoTexture.fx";
    mVertexShader.Initialize<VertexPX>(shaderFile);
    mPixelShader.Initialize(shaderFile);

    mDiffuseTexture.Initialize("../../Assets/Images/misc/basketball.jpg");
    mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
}

void GameState::Terminate()
{
    mSampler.Terminate();
    mDiffuseTexture.Terminate();
    mPixelShader.Terminate();
    mVertexShader.Terminate();
    mConstantBuffer.Terminate();
    mMeshBuffer.Terminate();
}

float gRotationY = 0.0f;
float gRotationX = 0.0f;
void GameState::Update(float deltaTime)
{
    gRotationY += Math::Constants::HalfPi * deltaTime * 0.5f;
    gRotationX += Math::Constants::HalfPi * deltaTime * 0.25f;
}

void GameState::Render()
{
    mVertexShader.Bind();
    mPixelShader.Bind();

    mDiffuseTexture.BindPS(0);
    mSampler.BindPS(0);

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