#include "Graphics/GPUBuffer/RIndexBuffer.h"



RIndexBuffer::RIndexBuffer() :
	mIndexCount(0)
{

}

RIndexBuffer::~RIndexBuffer()
{

}

bool RIndexBuffer::Create(const vector<UINT>& indices)
{
	mIndexCount = (UINT)indices.size();

	desc.ByteWidth = sizeof(UINT) * (UINT)indices.size();
	desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sub = {};
	sub.pSysMem = indices.data();

	if (!GetDevice()->CreateBuffer(&desc, &sub, buffer.GetAddressOf()))
		assert(NULL && "indices buffer create fail!!");

	return true;
}

void RIndexBuffer::Bind() const
{
	GetDevice()->BindIndexBuffer(buffer.Get(), DXGI_FORMAT_R32_UINT, 0);
}
