#pragma once

#include <FlowerEngine/Inc/FlowerEngine.h>

using namespace FlowerEngine;
using namespace FlowerEngine::Math;
using namespace FlowerEngine::Graphics;
using namespace FlowerEngine::Core;
using namespace FlowerEngine::Input;

enum class SolarSystemDraws
{
    Sun,
    Mercury,
    Venus,
    Earth,
    Mars,
    Jupiter,
    Saturn,
    Uranus,
    Neptune,
    Pluto
};


class SolarSystem
{
public:
    //Rendering
    FlowerEngine::Graphics::MeshBuffer mMeshBuffer;
    FlowerEngine::Graphics::Texture mDiffuseTexture;

    //Planet's properties
    Matrix4 mMatWorld;
    float mDistanceFromSun;
    float mRotationSpeed;
    float mOrbitSpeed;

    float rotation;
    float orbit;

    void Initialize(const char* texturePath, MeshPX& mesh,float distance, float rotationSpeed, float orbitSpeed)
    {
        mDiffuseTexture.Initialize(texturePath);
        mMeshBuffer.Initialize<MeshPX>(mesh);

        mDistanceFromSun = distance;
        mRotationSpeed = rotationSpeed;
        mOrbitSpeed = orbitSpeed;

        mMatWorld = Matrix4::Identity;
    }

    void Terminate()
    {
        mMeshBuffer.Terminate();
        mDiffuseTexture.Terminate();
    }

    void Update(float deltaTime)
    {
        rotation += mRotationSpeed * deltaTime;
        orbit += mOrbitSpeed * deltaTime;

        // Rotation on own axis
        Matrix4 rotationAxis = Matrix4::RotationY(rotation);
        Matrix4 orbitAxis = Matrix4::RotationY(orbit);

        // Rotation/Orbit around the sun
        Matrix4 translation = Matrix4::Translation(Vector3::ZAxis * mDistanceFromSun);
        
        mMatWorld = rotationAxis * translation * orbitAxis;
    }

    void Render(ConstantBuffer& constantBuffer, const Matrix4& matView, const Matrix4& matProjection)
    {
        Matrix4 matFinal = mMatWorld * matView * matProjection;
        Matrix4 wvp = Transpose(matFinal);

        constantBuffer.Update(&wvp);
        constantBuffer.BindVS(0);

        mDiffuseTexture.BindPS(0);
        mMeshBuffer.Render();
    }

    Vector3 GetPlanetPosition() const
    {
        return Vector3(mMatWorld._41, mMatWorld._42, mMatWorld._43);
    }
};

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

    SolarSystem mSolarSystem[10];

    FlowerEngine::Graphics::Camera mCamera;
    FlowerEngine::Graphics::Camera mRenderTargetCamera;
    FlowerEngine::Graphics::ConstantBuffer mConstantBuffer;
    FlowerEngine::Graphics::MeshBuffer mMeshBuffer;
    FlowerEngine::Graphics::VertexShader mVertexShader;
    FlowerEngine::Graphics::PixelShader mPixelShader;
    FlowerEngine::Graphics::Texture mDiffuseTexture;
    FlowerEngine::Graphics::Sampler mSampler;
    FlowerEngine::Graphics::RenderTarget mRenderTarget;

    SolarSystemDraws mSolarSystemDraw = SolarSystemDraws::Sun;
};