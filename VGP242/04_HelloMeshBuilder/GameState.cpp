#include "GameState.h"

using namespace FlowerEngine;
using namespace FlowerEngine::Math;
using namespace FlowerEngine::Graphics;
using namespace FlowerEngine::Core;
using namespace FlowerEngine::Input;



void GameState::Initialize()
{
    // create a simple shape in NDC space (-1/1, -1/1, 0/1)

    //MeshPX mesh = MeshBuilder::CreateSkySpherePX(30, 30, 100.0f);
    CreateMesh();

    mCamera.SetPosition({ 0.0f, 1.0f, -3.0f });
    mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });

    //mMeshBuffer.Initialize<MeshPX>(mesh);

    //mConstantBuffer.Intialize(sizeof(Matrix4));

    //shaderFile = L"../../Assets/Shaders/DoTransform.fx";
    //shaderFile = L"../../Assets/Shaders/DoTexture.fx";
    //mVertexShader.Initialize<VertexPX>(shaderFile);
    //mPixelShader.Initialize(shaderFile);

    //mDiffuseTexture.Initialize("../../Assets/Images/skysphere/space.jpg");
    //mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
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
    UpdateCamera(deltaTime);
}

void GameState::UpdateCamera(float deltaTime)
{
    auto input = InputSystem::Get();
    const float moveSpeed = (input->IsKeyDown(KeyCode::LSHIFT) ? 10.0f : 1.0f) * deltaTime;
    const float turnSpeed = 0.1f * deltaTime;
    if (input->IsKeyDown(KeyCode::W))
    {
        mCamera.Walk(moveSpeed);
    }
    else if (input->IsKeyDown(KeyCode::S))
    {
        mCamera.Walk(-moveSpeed);
    }
    if (input->IsKeyDown(KeyCode::D))
    {
        mCamera.Strafe(moveSpeed);
    }
    else if (input->IsKeyDown(KeyCode::A))
    {
        mCamera.Strafe(-moveSpeed);
    }
    if (input->IsKeyDown(KeyCode::E))
    {
        mCamera.Rise(moveSpeed);
    }
    else if (input->IsKeyDown(KeyCode::Q))
    {
        mCamera.Rise(-moveSpeed);
    }
    if (input->IsMouseDown(MouseButton::RBUTTON))
    {
        mCamera.Yaw(input->GetMouseMoveX() * turnSpeed);
        mCamera.Pitch(input->GetMouseMoveY() * turnSpeed);
    }
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

//----------------------------------CUBE------------------------------------
void CubeMesh::Update(float deltaTime)
{
    UpdateCamera(deltaTime);
    if (InputSystem::Get()->IsKeyPressed(KeyCode::TWO))
    {
        MainApp().ChangeState("RectMesh");
    }
    if (InputSystem::Get()->IsKeyPressed(KeyCode::THREE))
    {
        MainApp().ChangeState("PlaneMesh");
    }
    if (InputSystem::Get()->IsKeyPressed(KeyCode::FOUR))
    {
        MainApp().ChangeState("CylinderMesh");
    }
    if (InputSystem::Get()->IsKeyPressed(KeyCode::FIVE))
    {
        MainApp().ChangeState("SphereMesh");
    }
    if (InputSystem::Get()->IsKeyPressed(KeyCode::SIX))
    {
        MainApp().ChangeState("SkyBoxMesh");
    }
    if (InputSystem::Get()->IsKeyPressed(KeyCode::SEVEN))
    {
        MainApp().ChangeState("SkySpehereMesh");
    }
}

void CubeMesh::CreateMesh()
{
    MeshPC mesh = MeshBuilder::CreateCubePC(1.0f);

    mMeshBuffer.Initialize<MeshPC>(mesh);

    mConstantBuffer.Intialize(sizeof(Matrix4));

    shaderFile = L"../../Assets/Shaders/DoTransform.fx";

    mVertexShader.Initialize<VertexPC>(shaderFile);
    mPixelShader.Initialize(shaderFile);
}

//----------------------------------RECT------------------------------------
void RectMesh::Update(float deltaTime)
{
    UpdateCamera(deltaTime);
    if (InputSystem::Get()->IsKeyPressed(KeyCode::ONE))
    {
        MainApp().ChangeState("CubeMesh");
    }
    if (InputSystem::Get()->IsKeyPressed(KeyCode::THREE))
    {
        MainApp().ChangeState("PlaneMesh");
    }
    if (InputSystem::Get()->IsKeyPressed(KeyCode::FOUR))
    {
        MainApp().ChangeState("CylinderMesh");
    }
    if (InputSystem::Get()->IsKeyPressed(KeyCode::FIVE))
    {
        MainApp().ChangeState("SphereMesh");
    }
    if (InputSystem::Get()->IsKeyPressed(KeyCode::SIX))
    {
        MainApp().ChangeState("SkyBoxMesh");
    }
    if (InputSystem::Get()->IsKeyPressed(KeyCode::SEVEN))
    {
        MainApp().ChangeState("SkySpehereMesh");
    }
}

void RectMesh::CreateMesh()
{
    MeshPC mesh = MeshBuilder::CreateRectPC(1.0f, 1.5f, 2.0f);

    mMeshBuffer.Initialize<MeshPC>(mesh);

    mConstantBuffer.Intialize(sizeof(Matrix4));

    shaderFile = L"../../Assets/Shaders/DoTransform.fx";

    mVertexShader.Initialize<VertexPC>(shaderFile);
    mPixelShader.Initialize(shaderFile);
}

//----------------------------------PLANE------------------------------------
void PlaneMesh::Update(float deltaTime)
{
    UpdateCamera(deltaTime);
    if (InputSystem::Get()->IsKeyPressed(KeyCode::ONE))
    {
        MainApp().ChangeState("CubeMesh");
    }
    if (InputSystem::Get()->IsKeyPressed(KeyCode::TWO))
    {
        MainApp().ChangeState("RectMesh");
    }
    if (InputSystem::Get()->IsKeyPressed(KeyCode::FOUR))
    {
        MainApp().ChangeState("CylinderMesh");
    }
    if (InputSystem::Get()->IsKeyPressed(KeyCode::FIVE))
    {
        MainApp().ChangeState("SphereMesh");
    }
    if (InputSystem::Get()->IsKeyPressed(KeyCode::SIX))
    {
        MainApp().ChangeState("SkyBoxMesh");
    }
    if (InputSystem::Get()->IsKeyPressed(KeyCode::SEVEN))
    {
        MainApp().ChangeState("SkySpehereMesh");
    }
}

void PlaneMesh::CreateMesh()
{
    MeshPC mesh = MeshBuilder::CreatePlanePC(2, 3, 0.5f);

    mMeshBuffer.Initialize<MeshPC>(mesh);

    mConstantBuffer.Intialize(sizeof(Matrix4));

    shaderFile = L"../../Assets/Shaders/DoTransform.fx";

    mVertexShader.Initialize<VertexPC>(shaderFile);
    mPixelShader.Initialize(shaderFile);
}

//----------------------------------CYLINDER------------------------------------
void CylinderMesh::Update(float deltaTime)
{
    UpdateCamera(deltaTime);
    if (InputSystem::Get()->IsKeyPressed(KeyCode::ONE))
    {
        MainApp().ChangeState("CubeMesh");
    }
    if (InputSystem::Get()->IsKeyPressed(KeyCode::TWO))
    {
        MainApp().ChangeState("RectMesh");
    }
    if (InputSystem::Get()->IsKeyPressed(KeyCode::THREE))
    {
        MainApp().ChangeState("PlaneMesh");
    }
    if (InputSystem::Get()->IsKeyPressed(KeyCode::FIVE))
    {
        MainApp().ChangeState("SphereMesh");
    }
    if (InputSystem::Get()->IsKeyPressed(KeyCode::SIX))
    {
        MainApp().ChangeState("SkyBoxMesh");
    }
    if (InputSystem::Get()->IsKeyPressed(KeyCode::SEVEN))
    {
        MainApp().ChangeState("SkySpehereMesh");
    }
}

void CylinderMesh::CreateMesh()
{
    MeshPC mesh = MeshBuilder::CreateCylinderPC(8, 1);

    mMeshBuffer.Initialize<MeshPC>(mesh);

    mConstantBuffer.Intialize(sizeof(Matrix4));

    shaderFile = L"../../Assets/Shaders/DoTransform.fx";

    mVertexShader.Initialize<VertexPC>(shaderFile);
    mPixelShader.Initialize(shaderFile);
}

//----------------------------------SPHERE------------------------------------
void SphereMesh::Update(float deltaTime)
{
    UpdateCamera(deltaTime);
    if (InputSystem::Get()->IsKeyPressed(KeyCode::ONE))
    {
        MainApp().ChangeState("CubeMesh");
    }
    if (InputSystem::Get()->IsKeyPressed(KeyCode::TWO))
    {
        MainApp().ChangeState("RectMesh");
    }
    if (InputSystem::Get()->IsKeyPressed(KeyCode::THREE))
    {
        MainApp().ChangeState("PlaneMesh");
    }
    if (InputSystem::Get()->IsKeyPressed(KeyCode::FOUR))
    {
        MainApp().ChangeState("CylinderMesh");
    }
    if (InputSystem::Get()->IsKeyPressed(KeyCode::SIX))
    {
        MainApp().ChangeState("SkyBoxMesh");
    }
    if (InputSystem::Get()->IsKeyPressed(KeyCode::SEVEN))
    {
        MainApp().ChangeState("SkySpehereMesh");
    }
}

void SphereMesh::CreateMesh()
{
    MeshPC mesh = MeshBuilder::CreateSpherePC(20, 20, 2.5f);

    mMeshBuffer.Initialize<MeshPC>(mesh);

    mConstantBuffer.Intialize(sizeof(Matrix4));

    shaderFile = L"../../Assets/Shaders/DoTransform.fx";

    mVertexShader.Initialize<VertexPC>(shaderFile);
    mPixelShader.Initialize(shaderFile);
}

//----------------------------------SKYBOX------------------------------------
void SkyBoxMesh::Update(float deltaTime)
{
    UpdateCamera(deltaTime);
    if (InputSystem::Get()->IsKeyPressed(KeyCode::ONE))
    {
        MainApp().ChangeState("CubeMesh");
    }
    if (InputSystem::Get()->IsKeyPressed(KeyCode::TWO))
    {
        MainApp().ChangeState("RectMesh");
    }
    if (InputSystem::Get()->IsKeyPressed(KeyCode::THREE))
    {
        MainApp().ChangeState("PlaneMesh");
    }
    if (InputSystem::Get()->IsKeyPressed(KeyCode::FOUR))
    {
        MainApp().ChangeState("CylinderMesh");
    }
    if (InputSystem::Get()->IsKeyPressed(KeyCode::FIVE))
    {
        MainApp().ChangeState("SphereMesh");
    }
    if (InputSystem::Get()->IsKeyPressed(KeyCode::SEVEN))
    {
        MainApp().ChangeState("SkySpehereMesh");
    }
}

void SkyBoxMesh::CreateMesh()
{
    MeshPX mesh = MeshBuilder::CreateSkyboxPX(100.0f);

    mMeshBuffer.Initialize<MeshPX>(mesh);

    mConstantBuffer.Intialize(sizeof(Matrix4));

    shaderFile = L"../../Assets/Shaders/DoTexture.fx";

    mVertexShader.Initialize<VertexPC>(shaderFile);
    mPixelShader.Initialize(shaderFile);

    mDiffuseTexture.Initialize("../../Assets/Images/skybox/skybox_texture.jpg");
    mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
}

//----------------------------------SKYSPHERE------------------------------------
void SkySphereMesh::Update(float deltaTime)
{
    UpdateCamera(deltaTime);
    if (InputSystem::Get()->IsKeyPressed(KeyCode::ONE))
    {
        MainApp().ChangeState("CubeMesh");
    }
    if (InputSystem::Get()->IsKeyPressed(KeyCode::TWO))
    {
        MainApp().ChangeState("RectMesh");
    }
    if (InputSystem::Get()->IsKeyPressed(KeyCode::THREE))
    {
        MainApp().ChangeState("PlaneMesh");
    }
    if (InputSystem::Get()->IsKeyPressed(KeyCode::FOUR))
    {
        MainApp().ChangeState("CylinderMesh");
    }
    if (InputSystem::Get()->IsKeyPressed(KeyCode::FIVE))
    {
        MainApp().ChangeState("SphereMesh");
    }
    if (InputSystem::Get()->IsKeyPressed(KeyCode::SIX))
    {
        MainApp().ChangeState("SkyBoxMesh");
    }
}

void SkySphereMesh::CreateMesh()
{

    MeshPX mesh = MeshBuilder::CreateSkySpherePX(30, 30, 100.0f);

    mMeshBuffer.Initialize<MeshPX>(mesh);

    mConstantBuffer.Intialize(sizeof(Matrix4));

    shaderFile = L"../../Assets/Shaders/DoTexture.fx";
    mVertexShader.Initialize<VertexPX>(shaderFile);
    mPixelShader.Initialize(shaderFile);

    mDiffuseTexture.Initialize("../../Assets/Images/skysphere/space.jpg");
    mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
}