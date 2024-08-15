#pragma once

#include "MeshTypes.h"

namespace FlowerEngine::Graphics
{
	class MeshBuilder
	{
	public:
		// Cube
		static MeshPC CreateCubePC(float size);
		
		// Rectangle
		static MeshPC CreateRectPC(float width, float length, float height);
	};
}