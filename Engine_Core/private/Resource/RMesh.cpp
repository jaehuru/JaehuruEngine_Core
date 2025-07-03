#include "Resource/RMesh.h"


RMesh::Data::Data()
	: mTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
	, vertices{}
	, indices{}
{

}

RMesh::Data::~Data()
{

}

RMesh::RMesh()
	: RResource(EResourceType::RMesh)
{

}

RMesh::~RMesh()
{

}

HRESULT RMesh::Save(const wstring& path)
{
	return S_OK;
}

HRESULT RMesh::Load(const wstring& path)
{

	return S_OK;
}

bool RMesh::CreateVB(const vector<FVertex>& vertices)
{
	mData.vertices = vertices;
	return mVB.Create(vertices);
}

bool RMesh::CreateIB(const vector<UINT>& indices)
{
	mData.indices = indices;
	return mIB.Create(indices);
}

void RMesh::SetVertexBufferParams(UINT vertexCount, D3D11_INPUT_ELEMENT_DESC* layout, const void* pShaderBytecodeWithInputSignature, SIZE_T BytecodeLength)
{
	mInputLayout.CreateInputLayout(vertexCount, layout, pShaderBytecodeWithInputSignature, BytecodeLength);
}

void RMesh::Bind()
{
	mInputLayout.Bind();

	mVB.Bind();
	mIB.Bind();

	GetDevice()->BindPrimitiveTopology(mData.mTopology);
}
