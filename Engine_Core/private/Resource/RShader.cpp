#include "Resource/RShader.h"
#include "Renderer/RRenderer.h"
#include <string>
#include <locale>
#include <codecvt>


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

void RShader::Serialize(json& jsonObject) const
{
    RResource::Serialize(jsonObject);
    jsonObject["RasterizerState"] = static_cast<int>(mRasterizerState);
    jsonObject["BlendState"] = static_cast<int>(mBlendState);
    jsonObject["DepthStencilState"] = static_cast<int>(mDepthStencilState);

    json shaderPathsArray = json::array();
    wstring_convert<codecvt_utf8<wchar_t>, wchar_t> converter;
    for (const auto& pair : mShaderPaths)
    {
        json pathEntry;
        pathEntry["Stage"] = static_cast<int>(pair.first);
        pathEntry["Path"] = converter.to_bytes(pair.second);
        shaderPathsArray.push_back(pathEntry);
    }
    jsonObject["ShaderPaths"] = shaderPathsArray;
}

void RShader::Deserialize(const json& jsonObject)
{
    RResource::Deserialize(jsonObject);
    mRasterizerState = static_cast<ERasterizerState>(jsonObject["RasterizerState"]);
    mBlendState = static_cast<EBlendState>(jsonObject["BlendState"]);
    mDepthStencilState = static_cast<EDepthStencilState>(jsonObject["DepthStencilState"]);

    const json& shaderPathsArray = jsonObject["ShaderPaths"];
    wstring_convert<codecvt_utf8<wchar_t>, wchar_t> converter;
    for (const auto& pathEntry : shaderPathsArray)
    {
        EShaderStage stage = static_cast<EShaderStage>(pathEntry["Stage"]);
        wstring path = converter.from_bytes(pathEntry["Path"]);
        mShaderPaths[stage] = path;
    }
}

bool RShader::Create(const EShaderStage stage, const wstring& fullPath)
{
	switch (stage)
	{
	case EShaderStage::VS:
		return CreateVertexShader(fullPath);
	case EShaderStage::PS:
		return CreatePixelShader(fullPath);
		// Geometry, Compute
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

	GetDevice()->BindRasterizerState(renderer::rasterizerStates[(UINT)mRasterizerState].Get());
	GetDevice()->BindBlendState(renderer::blendStates[(UINT)mBlendState].Get(), nullptr, 0xffffff);
	GetDevice()->BindDepthStencilState(renderer::depthStencilStates[(UINT)mDepthStencilState].Get(), 0);
}