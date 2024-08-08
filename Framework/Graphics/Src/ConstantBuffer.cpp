#include "Precompiled.h"
#include "ConstantBuffer.h"

#include "GraphicsSystem.h"

using namespace FlowerEngine;
using namespace FlowerEngine::Graphics;

ConstantBuffer::~ConstantBuffer()
{
	ASSERT(mConstantBuffer == nullptr, "ConstantBuffer: terminate must be called");
}

void ConstantBuffer::Intialize(uint32_t bufferSize)
{
	D3D11_BUFFER_DESC desc{};
	desc.ByteWidth = bufferSize;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = 0;
	desc.StructureByteStride = 0;

	auto device = GraphicsSystem::Get()->GetDevice();
	HRESULT hr = device->CreateBuffer(&desc, nullptr, &mConstantBuffer);
	ASSERT(SUCCEEDED(hr), "ConstantBuffer: failed to create buffer");
}

void ConstantBuffer::Terminate()
{

}

void ConstantBuffer::Update(const void* data) const
{

}

void ConstantBuffer::BindVS(uint32_t slot) const
{

}

void ConstantBuffer::BindPS(uint32_t slot) const
{

}
