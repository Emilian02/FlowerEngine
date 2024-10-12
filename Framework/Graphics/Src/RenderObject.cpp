#include "Precompiled.h"
#include "RenderObject.h"

using namespace FlowerEngine;
using namespace FlowerEngine::Graphics;

void RenderObject::Terminate()
{
    meshBuffer.Terminate();
}
