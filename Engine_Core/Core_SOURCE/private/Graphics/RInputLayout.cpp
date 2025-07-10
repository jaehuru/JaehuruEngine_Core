#include "Graphics/RInputLayout.h"


RInputLayout::RInputLayout() : 
	mInputLayout(nullptr)
{

}

RInputLayout::~RInputLayout()
{

}

void RInputLayout::CreateInputLayout(UINT vertexCount, D3D11_INPUT_ELEMENT_DESC* layout
	, const void* pShaderBytecodeWithInputSignature, SIZE_T BytecodeLength)
{
	if (!(GetDevice()->CreateInputLayout(layout, vertexCount
		, pShaderBytecodeWithInputSignature
		, BytecodeLength
		, mInputLayout.GetAddressOf())))
		assert(NULL && "Create input layout failed!");
}

void RInputLayout::Bind()
{
	if (mInputLayout)
		GetDevice()->BindInputLayout(mInputLayout.Get());
}
