#pragma once

#include "Resource/RResource.h"
#include "Graphics/RInputLayout.h"
#include "Graphics/GPUBuffer/RVertexBuffer.h"
#include "Graphics/GPUBuffer/RIndexBuffer.h"


class RMesh : public RResource
{
public:
	struct Data
	{
		Data();
		~Data();

		D3D11_PRIMITIVE_TOPOLOGY mTopology;
		vector<FVertex> vertices;
		vector<UINT> indices;
	};

	RMesh();
	~RMesh();

	virtual HRESULT Save(const wstring& path) override;
	virtual HRESULT Load(const wstring& path) override;


	bool	CreateVB(const vector<FVertex>& vertices);
	bool	CreateIB(const vector<UINT>& indices);
	void	SetVertexBufferParams(UINT vertexCount,
								D3D11_INPUT_ELEMENT_DESC* layout,
								const void* pShaderBytecodeWithInputSignature,
								SIZE_T BytecodeLength);

	void	Bind();

	UINT	GetIndexCount() const	{ return mIB.GetIndexCount(); }

private:
	RInputLayout			mInputLayout;
	RVertexBuffer		mVB;
	RIndexBuffer			mIB;

	Data				mData;

};


