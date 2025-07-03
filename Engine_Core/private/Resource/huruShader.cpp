#include "Resource/huruShader.h"
#include "Renderer/huruRenderer.h"

namespace huru::graphics
{
	Shader::Shader() : 
		Resource(eResourceType::Shader),
		mRasterizerState(eRasterizerState::SolidBack),
		mBlendState(eBlendState::AlphaBlend),
		mDepthStencilState(eDepthStencilState::LessEqual)
	{

	}

	Shader::~Shader()
	{

	}

	HRESULT Shader::Save(const wstring& path)
	{
		return E_NOTIMPL;
	}

	HRESULT Shader::Load(const wstring& path)
	{
		assert(false && "Shader::Load(const wstring&) is not implemented. Use Load(const map<eShaderStage, wstring>&) instead.");
		return E_NOTIMPL;
	}

	HRESULT Shader::Load(const map<eShaderStage, wstring>& shaderPaths)
	{
		for (const auto& [stage, path] : shaderPaths)
		{
			if (!Create(stage, path))
				return S_FALSE;
		}
		return S_OK;
	}

	bool Shader::Create(const eShaderStage stage, const wstring& fullPath)
	{
		switch (stage)
		{
		case eShaderStage::VS:
			return CreateVertexShader(fullPath);
		case eShaderStage::PS:
			return CreatePixelShader(fullPath);
			// Geometry, Compute 등 확장 가능
		default:
			assert(false && "Invalid Shader Stage");
			return false;
		}
	}

	bool Shader::CreateVertexShader(const wstring& fullPath)
	{
		return GetDevice()->CreateVertexShader(fullPath, mVSBlob.GetAddressOf(), mVS.GetAddressOf());
	}

	bool Shader::CreatePixelShader(const wstring& fullPath)
	{
		return GetDevice()->CreatePixelShader(fullPath, mPSBlob.GetAddressOf(), mPS.GetAddressOf());
	}

	void Shader::Bind()
	{
		if (mVS)
			GetDevice()->BindVS(mVS.Get());
		if (mPS)
			GetDevice()->BindPS(mPS.Get());

		GetDevice()->BindRasterizerState(renderer::rasterizerStates[(UINT)mRasterizerState].Get());
		GetDevice()->BindBlendState(renderer::blendStates[(UINT)mBlendState].Get(), nullptr, 0xffffff);
		GetDevice()->BindDepthStencilState(renderer::depthStencilStates[(UINT)mDepthStencilState].Get(), 0);
	}
}