#include "Precompiled.h"
#include "UISpriteRenderer.h"

#include "UISprite.h"

#include "GraphicsSystem.h"

using namespace FlowerEngine;
using namespace FlowerEngine::Graphics;

namespace
{
    std::unique_ptr<UISpriteRenderer> sSpriteRenderer;
}

void UISpriteRenderer::StaticInitialize()
{
    ASSERT(sSpriteRenderer == nullptr, "UISpriteRenderer: is already initialized");
    sSpriteRenderer = std::make_unique<UISpriteRenderer>();
    sSpriteRenderer->Initialize();
}

void UISpriteRenderer::StaticTerminate()
{
    if (sSpriteRenderer != nullptr)
    {
        sSpriteRenderer->Terminate();
        sSpriteRenderer.reset();
    }
}

UISpriteRenderer* UISpriteRenderer::Get()
{
    ASSERT(sSpriteRenderer != nullptr, "UISpriteRenderer: is not initialized");
    return sSpriteRenderer.get();
}

UISpriteRenderer::~UISpriteRenderer()
{
    ASSERT(mSpriteBatch == nullptr, "UISpriteRenderer: termiante must be called");
}

void UISpriteRenderer::Initialize()
{
    GraphicsSystem* gs = GraphicsSystem::Get();
    mCommonState = new DirectX::CommonStates(gs->GetDevice());
    mSpriteBatch = new DirectX::SpriteBatch(gs->GetContext());
}

void UISpriteRenderer::Terminate()
{
    if (mCommonState != nullptr)
    {
        delete mCommonState;
        mCommonState = nullptr;
    }
    if (mSpriteBatch != nullptr)
    {
        delete mSpriteBatch;
        mSpriteBatch = nullptr;
    }
}

void UISpriteRenderer::BeginRender()
{
    mSpriteBatch->Begin(DirectX::SpriteSortMode_Deferred, mCommonState->NonPremultiplied());
}

void UISpriteRenderer::EndRender()
{
    mSpriteBatch->End();

    // restore the state object
    auto blendState = mCommonState->Opaque();
    auto depthStencilState = mCommonState->DepthDefault();
    auto rasterizerState = mCommonState->CullCounterClockwise();

    auto context = GraphicsSystem::Get()->GetContext();
    context->OMSetBlendState(blendState, nullptr, 0xFFFFFFFF);
    context->OMSetDepthStencilState(depthStencilState, 0);
    context->RSSetState(rasterizerState);
}

void UISpriteRenderer::Render(const UISprite& uiSprite)
{
    const Texture* texture = TextureCache::Get()->GetTexture(uiSprite.mTextureId);
    if (texture != nullptr)
    {
        mSpriteBatch->Draw(
            (ID3D11ShaderResourceView*)texture->GetRawData(),
            uiSprite.mPosition,
            &uiSprite.mRect,
            uiSprite.mColor,
            uiSprite.mRotation,
            uiSprite.mOrigin,
            uiSprite.mScale,
            uiSprite.mFlip
        );
    }
}
