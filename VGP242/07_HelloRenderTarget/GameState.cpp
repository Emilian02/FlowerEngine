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
     "../../Assets/Images/Planets/sun.jpg",
     "../../Assets/Images/Planets/mercury.jpg",
     "../../Assets/Images/Planets/venus.jpg",
     "../../Assets/Images/Planets/earth.jpg",
     "../../Assets/Images/Planets/mars.jpg",
     "../../Assets/Images/Planets/jupiter.jpg",
     "../../Assets/Images/Planets/saturn.jpg",
     "../../Assets/Images/Planets/uranus.jpg",
     "../../Assets/Images/Planets/neptune.jpg",
     "../../Assets/Images/Planets/pluto.jpg"
};

float planetDistances[] =
{
    0.0f,   // Sun
    10.0f,  // Mercury
    20.0f,  // Venus
    30.0f,  // Earth
    40.0f,  // Mars
    80.0f,  // Jupiter
    100.0f, // Saturn
    120.0f, // Uranus
    140.0f, // Neptune
    160.0f  // Pluto
};

float rotationSpeeds[] =
{
    1.0f,   // Sun
    4.8f,   // Mercury
    -0.5f,  // Venus
    0.7f,   // Earth
    0.4f,   // Mars
    2.0f,   // Jupiter
    1.0f,   // Saturn
    -0.7f,  // Uranus
    0.5f,   // Neptune
    0.1f    // Pluto
};


float orbitSpeeds[] =
{
    0.0f,   // Sun
    4.8f,   // Mercury
    3.5f,   // Venus
    2.9f,   // Earth
    2.4f,   // Mars
    1.3f,   // Jupiter
    0.9f,   // Saturn
    0.6f,   // Uranus
    0.5f,   // Neptune
    0.4f    // Pluto
};


float planetSizes[] =
{
    5.0f, // Sun
    0.4f, // Mercury
    0.95f,// Venus
    1.0f, // Earth
    0.53f,// Mars
    11.2f,// Jupiter
    9.45f,// Saturn
    4.0f, // Uranus
    3.88f,// Neptune
    0.19f // Pluto
};


void GameState::Initialize()
{
    MeshPX mesh = MeshBuilder::CreateSkySpherePX(200, 200, 200.0f);

    mCamera.SetPosition({ 0.0f, 50.0f, -30.0f });
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

    for (int i = 0; i < 10; ++i)
    {
        MeshPX planetMesh = MeshBuilder::CreateSpherePX(50, 50, planetSizes[i]);

        mSolarSystem[i].Initialize(gTextures[i], planetMesh, planetDistances[i], rotationSpeeds[i], orbitSpeeds[i]);
    }

    constexpr uint32_t size = 512;
    mRenderTarget.Initialize(512, 512, Texture::Format::RGBA_U32);
}

void GameState::Terminate()
{
    mRenderTarget.Terminate();
    for (int i = 0; i < 10; ++i)
    {
        mSolarSystem[i].Terminate();
    }
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

    for (int i = 0; i < 10; ++i)
    {
        mSolarSystem[i].Update(deltaTime);
    }
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


bool ringToggle = true;
int planetIndex = 0;
float renderTargetDistance = -30.f;
void GameState::Render()
{

    for (int i = 0; i < 10; ++i)
    {
        if (ringToggle)
        {
            SimpleDraw::AddGroundCircle(100, mSolarSystem[i].mDistanceFromSun, { 0, 0, 0 }, Colors::White);
        }
    }
    SimpleDraw::Render(mCamera);

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

    mMeshBuffer.Render();

    for (int i = 0; i < 10; ++i)
    {
        mSolarSystem[i].Render(mConstantBuffer, matView, matProj);
    }

    Vector3 planetPosition = mSolarSystem[planetIndex].GetPlanetPosition();
    Vector3 offset(0.0f, 0.0f, renderTargetDistance);
    mRenderTargetCamera.SetPosition(planetPosition + offset);
    mRenderTargetCamera.SetLookAt(planetPosition);

    matWorld = mSolarSystem[planetIndex].mMatWorld;
    matView = mRenderTargetCamera.GetViewMatrix();
    matProj = mRenderTargetCamera.GetProjectionMatrix();
    matFinal = matWorld * matView * matProj;
    wvp = Transpose(matFinal);
    mConstantBuffer.Update(&wvp);
    mConstantBuffer.BindVS(0);

    mSolarSystem[planetIndex].mDiffuseTexture.BindPS(0);
    mRenderTarget.BeginRender();
        mSolarSystem[planetIndex].mMeshBuffer.Render();
    mRenderTarget.EndRender();

}


void GameState::DebugUI()
{
    SimpleDraw::Render(mCamera);
    ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    planetIndex = static_cast<int>(mSolarSystemDraw);

    if (ImGui::Combo("Planets", &planetIndex, gSolarSystem, static_cast<int>(std::size(gSolarSystem))))
    {
        mSolarSystemDraw = (SolarSystemDraws)planetIndex;
    }

    if (mSolarSystemDraw == SolarSystemDraws::Sun)
    {
        ImGui::DragFloat("Rotation Speed", &mSolarSystem[0].mRotationSpeed, 0.001f);
    }
    else if (mSolarSystemDraw == SolarSystemDraws::Mercury)
    {
        ImGui::DragFloat("Rotation Speed", &mSolarSystem[1].mRotationSpeed, 0.001f);
        ImGui::DragFloat("Orbit Speed", &mSolarSystem[1].mOrbitSpeed, 0.001f);
    }
    else if (mSolarSystemDraw == SolarSystemDraws::Venus)
    {
        ImGui::DragFloat("Rotation Speed", &mSolarSystem[2].mRotationSpeed, 0.001f);
        ImGui::DragFloat("Orbit Speed", &mSolarSystem[2].mOrbitSpeed, 0.001f);
    }
    else if (mSolarSystemDraw == SolarSystemDraws::Earth)
    {
        ImGui::DragFloat("Rotation Speed", &mSolarSystem[3].mRotationSpeed, 0.001f);
        ImGui::DragFloat("Orbit Speed", &mSolarSystem[3].mOrbitSpeed, 0.001f);
    }
    else if (mSolarSystemDraw == SolarSystemDraws::Mars)
    {
        ImGui::DragFloat("Rotation Speed", &mSolarSystem[4].mRotationSpeed, 0.001f);
        ImGui::DragFloat("Orbit Speed", &mSolarSystem[4].mOrbitSpeed, 0.001f);
    }
    else if (mSolarSystemDraw == SolarSystemDraws::Jupiter)
    {
        ImGui::DragFloat("Rotation Speed", &mSolarSystem[5].mRotationSpeed, 0.001f);
        ImGui::DragFloat("Orbit Speed", &mSolarSystem[5].mOrbitSpeed, 0.001f);
    }
    else if (mSolarSystemDraw == SolarSystemDraws::Saturn)
    {
        ImGui::DragFloat("Rotation Speed", &mSolarSystem[6].mRotationSpeed, 0.001f);
        ImGui::DragFloat("Orbit Speed", &mSolarSystem[6].mOrbitSpeed, 0.001f);
    }
    else if (mSolarSystemDraw == SolarSystemDraws::Uranus)
    {
        ImGui::DragFloat("Rotation Speed", &mSolarSystem[7].mRotationSpeed, 0.001f);
        ImGui::DragFloat("Orbit Speed", &mSolarSystem[7].mOrbitSpeed, 0.001f);
    }
    else if (mSolarSystemDraw == SolarSystemDraws::Neptune)
    {
        ImGui::DragFloat("Rotation Speed", &mSolarSystem[8].mRotationSpeed, 0.001f);
        ImGui::DragFloat("Orbit Speed", &mSolarSystem[8].mOrbitSpeed, 0.001f);
    }
    else if (mSolarSystemDraw == SolarSystemDraws::Pluto)
    {
        ImGui::DragFloat("Rotation Speed", &mSolarSystem[9].mRotationSpeed, 0.001f);
        ImGui::DragFloat("Orbit Speed", &mSolarSystem[9].mOrbitSpeed, 0.001f);
    }


    ImGui::Checkbox("Show Rings", &ringToggle);
    ImGui::DragFloat("Window Distance", &renderTargetDistance, 0.1f);

    ImGui::Image(
        mRenderTarget.GetRawData(),
        { 256, 256 },
        { 0, 0 },	//uv0
        { 1, 1 },	//uv1
        { 1, 1, 1, 1 },
        { 1, 1, 1, 1 });

    ImGui::End();
}