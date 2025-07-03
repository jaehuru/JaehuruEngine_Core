#include "Graphics/GPUBuffer/RVertexBuffer.h"


RVertexBuffer::RVertexBuffer()
{

}

RVertexBuffer::~RVertexBuffer()
{

}

bool RVertexBuffer::Create(const vector<FVertex>& vertexes)
{
	desc.ByteWidth = sizeof(FVertex) * (UINT)vertexes.size();
	desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
	desc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA sub = { };
	sub.pSysMem = vertexes.data();

	if (!(GetDevice()->CreateBuffer(&desc, &sub, buffer.GetAddressOf())))
		assert(NULL);

	return true;
}
void RVertexBuffer::Bind() const
{
	UINT offset = 0;
	UINT vectexSize = sizeof(FVertex);
	GetDevice()->BindVertexBuffer(0, 1, buffer.GetAddressOf(), &vectexSize, &offset);
}
