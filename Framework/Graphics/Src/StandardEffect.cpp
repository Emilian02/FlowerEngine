#include "Precompiled.h"
#include "StandardEffect.h"
#include "VertexTypes.h"
#include "Camera.h"
#include "RenderObject.h"

using namespace FlowerEngine;
using namespace FlowerEngine::Graphics;

void StandardEffect::Intitialize(const std::filesystem::path& path)
{
    mVertexShader.Initialize<VertexPX>(path);
    mPixelShader.Initialize(path);
    mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);

    mConstantBuffer.Intialize(sizeof(Math::Matrix4));
}

void StandardEffect::Terminate()
{
    mConstantBuffer.Terminate();
    mSampler.Terminate();
    mPixelShader.Terminate();
    mVertexShader.Terminate();
}

void StandardEffect::Begin()
{
    mVertexShader.Bind();
    mPixelShader.Bind();
    mSampler.BindPS(0);

    mConstantBuffer.BindVS(0);
}

void StandardEffect::End()
{
    // Shadows release texture
    // Blend clear blend state
}

void StandardEffect::Render(const RenderObject& renderObject)
{
    ASSERT(mCamera != nullptr, "StanderEffect: Must have a camera!");
    const Math::Matrix4 matWorld = renderObject.transform.GetMatrix4();
    const Math::Matrix4 matView = mCamera->GetViewMatrix();
    const Math::Matrix4 matProj = mCamera->GetProjectionMatrix();

    const Math::Matrix4 matFinal = matWorld * matView * matProj;
    const Math::Matrix4 wvp = Transpose(matFinal);
    mConstantBuffer.Update(&wvp);

    renderObject.diffuseTexture.BindPS(0);
    renderObject.meshBuffer.Render();
}

void StandardEffect::SetCamera(const Camera& camera)
{
    mCamera = &camera;
}

void StandardEffect::DebugUI()
{

}
