#include "Precompiled.h"
#include "SimpleDraw.h"

#include "VertexShader.h"
#include "PixelShader.h"
#include "ConstantBuffer.h"
#include "MeshBuffer.h"
#include "Camera.h"
#include "VertexTypes.h"

using namespace FlowerEngine;
using namespace FlowerEngine::Graphics;
using namespace FlowerEngine::Math;
using namespace FlowerEngine::Math::Constants;

namespace
{
    class SimpleDrawImpl
    {
    public:
        void Initialize(uint32_t maxVertexCount);
        void Terminate();

        void AddLine(const Vector3& v0, const Vector3& v1, const Color& color);
        void AddFace(const Vector3& v0, const Vector3& v1, const Vector3& v2, const Color& color);
        
        void Render(const Camera& camera);
    private:
        VertexShader mVertexShader;
        PixelShader mPixelShader;
        ConstantBuffer mConstantBuffer;
        MeshBuffer mMeshBuffer;

        std::unique_ptr<VertexPC[]> mLineVertices;
        std::unique_ptr<VertexPC[]> mFaceVertices;

        uint32_t mLineVertexCount = 0;
        uint32_t mFaceVertexCount = 0;
        uint32_t mMaxVertexCount = 0;
    };

    void SimpleDrawImpl::Initialize(uint32_t maxVertexCount)
    {
        std::filesystem::path shaderFile = "../../Assets/Shader/DoTransform.fx";
        mVertexShader.Initialize<VertexPC>(shaderFile);
        mPixelShader.Initialize(shaderFile);
        mConstantBuffer.Intialize(sizeof(Matrix4));
        mMeshBuffer.Initialize(nullptr, sizeof(VertexPC), maxVertexCount);

        mLineVertices = std::make_unique<VertexPC[]>(maxVertexCount);
        mFaceVertices = std::make_unique<VertexPC[]>(maxVertexCount);
        mLineVertexCount = 0;
        mFaceVertexCount = 0;
        mMaxVertexCount = maxVertexCount;
    }
    void SimpleDrawImpl::Terminate()
    {
        mMeshBuffer.Terminate();
        mConstantBuffer.Terminate();
        mPixelShader.Terminate();
        mVertexShader.Terminate();
    }
    void SimpleDrawImpl::AddLine(const Vector3& v0, const Vector3& v1, const Color& color)
    {
        if (mLineVertexCount + 2 <= mMaxVertexCount)
        {
            mLineVertices[mLineVertexCount++] = VertexPC{ v0, color };
            mLineVertices[mLineVertexCount++] = VertexPC{ v1, color };
        }
    }
    void SimpleDrawImpl::AddFace(const Vector3& v0, const Vector3& v1, const Vector3& v2, const Color& color)
    {
        if (mFaceVertexCount + 3 <= mMaxVertexCount)
        {
            mFaceVertices[mFaceVertexCount++] = VertexPC{ v0, color };
            mFaceVertices[mFaceVertexCount++] = VertexPC{ v1, color };
            mFaceVertices[mFaceVertexCount++] = VertexPC{ v2, color };
        }
    }
    void SimpleDrawImpl::Render(const Camera& camera)
    {
        const Matrix4 matView = camera.GetViewMatrix();
        const Matrix4 matProj = camera.GetProjectionMatrix();
        const Matrix4 transform = Transpose(matView * matProj);
        mConstantBuffer.Update(&transform);
        mConstantBuffer.BindVS(0);

        mVertexShader.Bind();
        mPixelShader.Bind();

        mMeshBuffer.Update(mFaceVertices.get(), mFaceVertexCount);
        mMeshBuffer.SetTopology(MeshBuffer::Topology::Triangles);
        mMeshBuffer.Render();

        mLineVertexCount = 0;
        mFaceVertexCount = 0;
    }

    std::unique_ptr<SimpleDrawImpl> sInstance;
}

void simpleDraw::StaticIntialize(uint32_t maxVertexCount)
{
    sInstance = std::make_unique<SimpleDrawImpl>();
    sInstance->Initialize(maxVertexCount);
}

void simpleDraw::StaticTerminate()
{
    sInstance->Terminate();
    sInstance.reset();
}

void simpleDraw::AddLine(const Math::Vector3& v0, const Math::Vector3 v1, const Color& color)
{
}

void simpleDraw::AddLine(const Math::Vector3& v0, const Math::Vector3 v1, const Math::Vector3 v2, const Color& color)
{
}

void simpleDraw::AddAABB(const Math::Vector3& min, const Math::Vector3& max, const Color& color)
{
}

void simpleDraw::AddAABB(float minX, float minY, float minZ, float maxX, float maxY, float maxZ, const Color& color)
{
}

void simpleDraw::AddFilledAABB(const Math::Vector3& min, const Math::Vector3& max, const Color& color)
{
}

void simpleDraw::AddFilledAABB(float minX, float minY, float minZ, float maxX, float maxY, float maxZ, const Color& color)
{
}

void simpleDraw::AddSphere(int slices, int rings, float radius, const Color& color)
{
}

void simpleDraw::AddGroundCircle(float slices, float radius, const Color& color)
{
}

void simpleDraw::AddGroundPlane(float size, const Color& color)
{
}

void simpleDraw::AddTransform(const Math::Matrix4 m)
{
}

void simpleDraw::Render(const Camera& camera)
{
}
