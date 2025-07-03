#include "Resource/RShader.h"
#include "Renderer/RRenderer.h"


RShader::RShader() : 
	RResource(EResourceType::RShader),
	mRasterizerState(ERasterizerState::SolidBack),
	mBlendState(EBlendState::AlphaBlend),
	mDepthStencilState(EDepthStencilState::LessEqual)
{

}

RShader::~RShader()
{

}

HRESULT RShader::Save(const wstring& path)
{
	return E_NOTIMPL;
}

HRESULT RShader::Load(const wstring& path)
{
	assert(false && "RShader::Load(const wstring&) is not implemented. Use Load(const map<eShaderStage, wstring>&) instead.");
	return E_NOTIMPL;
}

HRESULT RShader::Load(const map<EShaderStage, wstring>& shaderPaths)
{
	for (const auto& [stage, path] : shaderPaths)
	{
		if (!Create(stage, path))
			return S_FALSE;
	}
	return S_OK;
}

bool RShader::Create(const EShaderStage stage, const wstring& fullPath)
{
	switch (stage)
	{
	case EShaderStage::VS:
		return CreateVertexShader(fullPath);
	case EShaderStage::PS:
		return CreatePixelShader(fullPath);
		// Geometry, Compute 등 확장 가능
	default:
		assert(false && "Invalid RShader Stage");
		return false;
	}
}

bool RShader::CreateVertexShader(const wstring& fullPath)
{
	return GetDevice()->CreateVertexShader(fullPath, mVSBlob.GetAddressOf(), mVS.GetAddressOf());
}

bool RShader::CreatePixelShader(const wstring& fullPath)
{
	return GetDevice()->CreatePixelShader(fullPath, mPSBlob.GetAddressOf(), mPS.GetAddressOf());
}

void RShader::Bind()
{
	if (mVS)
		GetDevice()->BindVS(mVS.Get());
	if (mPS)
		GetDevice()->BindPS(mPS.Get());

	GetDevice()->BindRasterizerState(rasterizerStates[(UINT)mRasterizerState].Get());
	GetDevice()->BindBlendState(blendStates[(UINT)mBlendState].Get(), nullptr, 0xffffff);
	GetDevice()->BindDepthStencilState(depthStencilStates[(UINT)mDepthStencilState].Get(), 0);
}
