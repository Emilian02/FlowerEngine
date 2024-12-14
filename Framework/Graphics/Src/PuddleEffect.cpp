#include "Precompiled.h"
#include "PuddleEffect.h"

#include "GraphicsSystem.h"
#include "RenderObject.h"
#include "Camera.h"
#include "VertexTypes.h"

using namespace FlowerEngine;
using namespace FlowerEngine::Graphics;

void PuddleEffect::Initialize()
{
    std::filesystem::path shaderFile = L"../../Assets/Shaders/Puddle.fx";
    mVertexShader.Initialize<Vertex>(shaderFile);
    mPixelShader.Initialize(shaderFile);

    mTransformBuffer.Initialize();
    mLightBuffer.Initialize();
    mMaterialBuffer.Initialize();
    mSettingsBuffer.Initialize();
    mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);

    //mNoiseTexture.LoadTexture(L"../../Assets/Images/Terrain-Heights-Maps/terrain_map-0.jpg");
}

void PuddleEffect::Terminate()
{
    //mNoiseTexture.StaticTerminate();
    mSampler.Terminate();
    mSettingsBuffer.Terminate();
    mMaterialBuffer.Terminate();
    mLightBuffer.Terminate();
    mTransformBuffer.Terminate();
    mPixelShader.Terminate();
    mVertexShader.Terminate();
}

void PuddleEffect::Begin()
{
    mVertexShader.Bind();
    mPixelShader.Bind();

    mTransformBuffer.BindVS(0);

    mLightBuffer.BindVS(1);
    mLightBuffer.BindPS(1);

    mMaterialBuffer.BindPS(2);

    mSettingsBuffer.BindVS(3);
    mSettingsBuffer.BindPS(3);

    mSampler.BindVS(0);
    mSampler.BindPS(0);
}

void PuddleEffect::End()
{
}

void PuddleEffect::Render(const RenderObject& renderObject)
{
    ASSERT(mCamera != nullptr, "PuddleEffect: Must have a camera!");

    const Math::Matrix4 matWorld = renderObject.transform.GetMatrix4();
    const Math::Matrix4 matView = mCamera->GetViewMatrix();
    const Math::Matrix4 matProj = mCamera->GetProjectionMatrix();

    const Math::Matrix4 matFinal = matWorld * matView * matProj;

    TransformData transformData;
    transformData.wvp = Transpose(matFinal);
    transformData.world = Transpose(matWorld);
    transformData.viewPosition = mCamera->GetPosition();
    mTransformBuffer.Update(transformData);

    mSettingsBuffer.Update(mSettingsData);
    mLightBuffer.Update(*mDirectionalLight);
    mMaterialBuffer.Update(renderObject.material);

    TextureCache* tc = TextureCache::Get();
    tc->BindPS(renderObject.diffuseMapId, 0);
    tc->BindPS(renderObject.normalMapId, 1);
    tc->BindPS(renderObject.noiseMapId, 2);

    renderObject.meshBuffer.Render();
}

void PuddleEffect::DebugUI()
{
    if (ImGui::CollapsingHeader("PuddleEffect", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::DragFloat("Intensity", &mSettingsData.puddleIntensity, 0.001f);
        ImGui::DragFloat("Shininess", &mSettingsData.puddleShininess, 0.001f);
        ImGui::DragFloat("Blend Height", &mSettingsData.blendHeight, 0.001f);
        ImGui::DragFloat("Blend Thickness", &mSettingsData.blendThickness, 0.001f);
    }
}

void PuddleEffect::SetCamera(const Camera& camera)
{
    mCamera = &camera;
}

void PuddleEffect::SetDirectionalLight(const DirectionalLight& directionLight)
{
    mDirectionalLight = &directionLight;
}
