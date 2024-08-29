#pragma once


#include "Color.h"

namespace FlowerEngine::Graphics
{
    class Camera;

    namespace simpleDraw
    {
        void StaticIntialize(uint32_t maxVertexCount);
        void StaticTerminate();

        void AddLine(const Math::Vector3& v0, const Math::Vector3 v1, const Color& color);
        void AddLine(const Math::Vector3& v0, const Math::Vector3 v1, const Math::Vector3 v2, const Color& color);

        void AddAABB(const Math::Vector3& min, const Math::Vector3& max, const Color& color);
        void AddAABB(float minX, float minY, float minZ, float maxX, float maxY, float maxZ, const Color& color);

        void AddFilledAABB(const Math::Vector3& min, const Math::Vector3& max, const Color& color);
        void AddFilledAABB(float minX, float minY, float minZ, float maxX, float maxY, float maxZ, const Color& color);

        void AddSphere(int slices, int rings, float radius, const Color& color);
        void AddGroundCircle(float slices, float radius, const Color& color);

        void AddGroundPlane(float size, const Color& color);

        void AddTransform(const Math::Matrix4 m);

        void Render(const Camera& camera);
    }
}