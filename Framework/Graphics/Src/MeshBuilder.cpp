#include "Precompiled.h"

#include "MeshBuilder.h"

using namespace FlowerEngine;
using namespace FlowerEngine::Graphics;

namespace
{
    Color GetNextColor(int& index)
    {
        constexpr Color colorTable[] = {
            Colors::Red,
            Colors::Blue,
            Colors::Purple,
            Colors::Green,
            Colors::Yellow,
            Colors::Navy,
            Colors::SeaGreen
        };

        index = (index + 1) % std::size(colorTable);
        return colorTable[index];
    }
    
    void CreateCubeIndices(std::vector<uint32_t>& indices)
    {
        indices = {
            //front 
            0, 1, 2,
            0, 2, 3,
            //back
            7, 6, 5,
            7, 5, 4,
            //right
            3, 2, 6,
            3, 6, 7,
            //left
            0, 5, 1,
            0, 4, 5,
            //top
            1, 5, 6,
            1, 6, 2,
            //bottom
            0, 3, 7,
            0, 7, 4
        };
    }
}

MeshPC MeshBuilder::CreateCubePC(float size)
{
    MeshPC mesh;

    int index = rand() % 10;

    const float hs = size * 0.5f;

    // front
    mesh.vertices.push_back({ { -hs, -hs, -hs }, GetNextColor(index) });
    mesh.vertices.push_back({ { -hs,  hs, -hs }, GetNextColor(index) });
    mesh.vertices.push_back({ {  hs,  hs, -hs }, GetNextColor(index) });
    mesh.vertices.push_back({ {  hs, -hs, -hs }, GetNextColor(index) });

    // back
    mesh.vertices.push_back({ { -hs, -hs, hs }, GetNextColor(index) });
    mesh.vertices.push_back({ { -hs,  hs, hs }, GetNextColor(index) });
    mesh.vertices.push_back({ {  hs,  hs, hs }, GetNextColor(index) });
    mesh.vertices.push_back({ {  hs, -hs, hs }, GetNextColor(index) });

    CreateCubeIndices(mesh.indices);

    return mesh;
}

MeshPC MeshBuilder::CreateRectPC(float width, float length, float height)
{
    MeshPC mesh;

    int index = rand() % 10;

    const float hw = width * 0.5f;
    const float hh = height * 0.5f;
    const float hl = length * 0.5f;

    // front
    mesh.vertices.push_back({ { -hw, -hh, -hl }, GetNextColor(index) });
    mesh.vertices.push_back({ { -hw,  hh, -hl }, GetNextColor(index) });
    mesh.vertices.push_back({ {  hw,  hh, -hl }, GetNextColor(index) });
    mesh.vertices.push_back({ {  hw, -hh, -hl }, GetNextColor(index) });

    // back
    mesh.vertices.push_back({ { -hw, -hh, hl }, GetNextColor(index) });
    mesh.vertices.push_back({ { -hw,  hh, hl }, GetNextColor(index) });
    mesh.vertices.push_back({ {  hw,  hh, hl }, GetNextColor(index) });
    mesh.vertices.push_back({ {  hw, -hh, hl }, GetNextColor(index) });

    CreateCubeIndices(mesh.indices);

    return mesh;
}