#pragma once

#include "Resource/huruResource.h"
#include "Graphics/huruInputLayout.h"
#include "Graphics/GPUBuffer/huruVertexBuffer.h"
#include "Graphics/GPUBuffer/huruIndexBuffer.h"

namespace huru
{
	class Mesh : public Resource
	{
	public:
		struct Data
		{
			Data();
			~Data();

			D3D11_PRIMITIVE_TOPOLOGY mTopology;
			vector<Vertex> vertices;
			vector<UINT> indices;
		};

		Mesh();
		~Mesh();

		virtual HRESULT Save(const wstring& path) override;
		virtual HRESULT Load(const wstring& path) override;


		bool CreateVB(const vector<Vertex>& vertices);
		bool CreateIB(const vector<UINT>& indices);
		void SetVertexBufferParams(UINT vertexCount,
									D3D11_INPUT_ELEMENT_DESC* layout,
									const void* pShaderBytecodeWithInputSignature,
									SIZE_T BytecodeLength);

		void Bind();

	private:
		InputLayout			mInputLayout;
		VertexBuffer		mVB;
		IndexBuffer			mIB;

		Data				mData;

	};
}

