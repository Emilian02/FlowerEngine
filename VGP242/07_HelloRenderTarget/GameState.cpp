#include "GameState.h"

using namespace FlowerEngine;
using namespace FlowerEngine::Math;
using namespace FlowerEngine::Graphics;
using namespace FlowerEngine::Core;
using namespace FlowerEngine::Input;

const char* gSolarSystem[] =
{
    "Sun",
    "Mercury",
    "Venus",
    "Earth",
    "Mars",
    "Jupiter",
    "Saturn",
    "Uranus",
    "Neptune",
    "Pluto"
};

const char* gTextures[] =
{
    "sun.jpg",
    "mercury.jpg",
    "venus.jpg",
    "earth.jpg",
    "mars.jpg",
    "jupiter.jpg",
    "saturn.jpg",
    "uranus.jpg",
    "neptune.jpg",
    "pluto.jpg"
};

void GameState::Initialize()
{
    // planet::Init()
    //{
    MeshPX mesh = MeshBuilder::CreateSkySpherePX(200, 200, 200.0f);
    // meshBuffer.Init(mesh)
    // base transforms
    // matWorld = Matrix4::RotationMatrix(localRotation) * Matrix4::TranslationMatrix(vector3::zaxis * distanceFromSun) * Matrix4::RotationMatrix(orbitRotation)
    //}

    mCamera.SetPosition({ 0.0f, 1.0f, -3.0f });
    mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });

    mRenderTargetCamera.SetPosition({ 0.0f, 1.0f, -3.0f });
    mRenderTargetCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });
    mRenderTargetCamera.SetAspectRatio(1.0f);

    mMeshBuffer.Initialize<MeshPX>(mesh);

    mConstantBuffer.Intialize(sizeof(Matrix4));
    std::filesystem::path shaderFile = L"../../Assets/Shaders/DoTexture.fx";
    mVertexShader.Initialize<VertexPX>(shaderFile);
    mPixelShader.Initialize(shaderFile);

    mDiffuseTexture.Initialize("../../Assets/Images/skysphere/space.jpg");
    mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);

    // go through and initialize each planet
    // each planet needs MeshBuffer, Texture, Matrix4 (transform), rotation speed, distance from sun, update position/rotation

    constexpr uint32_t size = 512;
    mRenderTarget.Initialize(512, 512, Texture::Format::RGBA_U32);
}

void GameState::Terminate()
{
    mRenderTarget.Terminate();
    mSampler.Terminate();
    mDiffuseTexture.Terminate();
    mPixelShader.Terminate();
    mVertexShader.Terminate();
    mConstantBuffer.Terminate();
    mMeshBuffer.Terminate();
}

void GameState::Update(float deltaTime)
{
    UpdateCamera(deltaTime);

    // update all planet rotation/position
}

void GameState::UpdateCamera(float deltaTime)
{
    auto input = InputSystem::Get();
    const float moveSpeed = (input->IsKeyDown(KeyCode::LSHIFT) ? 10.0f : 1.0f) * deltaTime;
    const float turnSpeed = 0.2f * deltaTime;
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
    Matrix4 matWorld = Matrix4::Identity;
    Matrix4 matView = mCamera.GetViewMatrix();
    Matrix4 matProj = mCamera.GetProjectionMatrix();
    Matrix4 matFinal = matWorld * matView * matProj;
    Matrix4 wvp = Transpose(matFinal);
    mConstantBuffer.Update(&wvp);
    mConstantBuffer.BindVS(0);

    // render all planets
    // for(planet : mPlanets)
    // {
    //      matWorld = planet.matWorld; transform//
    //      matFinal = matWorld * matView * matProj
    //      wvp = Transpose(matFinal)
    // mConstantBuffer.Update(&wvp)
    // mConstantBuffer.BindVS(0)
    // planet.meshbuffer.render()
    //}
    mMeshBuffer.Render();

    matWorld = Matrix4::Identity;
    // option1===============================================
    // matView = planet.transform
    // mRenderTarget.setposition(planet.position + Normalize(planet.position) * 2.0f)
    // mRenderTarget.setlookat(planet.position)
    // option2===============================================
    // dont update renderTargetCamera
    // matWorld = Matrix4::identity
    // constant buffer stuff
    // rendertarget.begin()
    // planet[selectedPlanetIndex].meshBuffer.Render();
    // rendertarget.end()
    matView = mRenderTargetCamera.GetViewMatrix();
    matProj = mRenderTargetCamera.GetProjectionMatrix();
    matFinal = matWorld * matView * matProj;
    wvp = Transpose(matFinal);
    mConstantBuffer.Update(&wvp);
    mConstantBuffer.BindVS(0);

    mRenderTarget.BeginRender();
        mMeshBuffer.Render();
    mRenderTarget.EndRender();

}

bool buttonValue = false;
int intValue = 0;
void GameState::DebugUI()
{
    SimpleDraw::Render(mCamera);
    ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::End();
}