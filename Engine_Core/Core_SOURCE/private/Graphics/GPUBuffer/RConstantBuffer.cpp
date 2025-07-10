#include "Graphics/GPUBuffer/RConstantBuffer.h"


RConstantBuffer::RConstantBuffer(ECBType type) :
	mSize(0),
	mType(type)
{

}

RConstantBuffer::~RConstantBuffer()
{

}

bool RConstantBuffer::Create(UINT size, void* data)
{
	mSize = size;
	desc.ByteWidth = size;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA sub = {};
	sub.pSysMem = data;

	bool succes = false;
	if (data == NULL)
		succes = GetDevice()->CreateBuffer(&desc, nullptr, buffer.GetAddressOf());
	else
		succes = GetDevice()->CreateBuffer(&desc, &sub, buffer.GetAddressOf());

	if (!succes)
		assert(NULL);

	return true;
}

void RConstantBuffer::SetData(void* data) const
{
	GetDevice()->SetDataGpuBuffer(buffer.Get(), data, mSize);
}

void RConstantBuffer::Bind(EShaderStage stage) const
{
	GetDevice()->BindConstantBuffer(stage, mType, buffer.Get());
}
