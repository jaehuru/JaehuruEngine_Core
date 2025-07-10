#pragma once

#include "Graphics/RGraphicDevice_DX11.h"


class RInputLayout
{
public:
	RInputLayout();
	~RInputLayout();

	void CreateInputLayout(UINT vertexCount,
							D3D11_INPUT_ELEMENT_DESC* layout,
							const void* pShaderBytecodeWithInputSignature,
							SIZE_T BytecodeLength);
	void Bind();

private:
	Microsoft::WRL::ComPtr<ID3D11InputLayout> mInputLayout;
};


