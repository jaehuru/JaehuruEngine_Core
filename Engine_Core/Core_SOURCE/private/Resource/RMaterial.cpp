#include "Resource/RMaterial.h"
#include "Resource/RShader.h"
#include "Resource/RResources.h"


RMaterial::RMaterial() :
	RResource(EResourceType::RMaterial),
	mMode(ERenderingMode::Opaque),
	mAlbedoTexture(nullptr),
	mShader(nullptr)
{

}

RMaterial::~RMaterial()
{
}

HRESULT RMaterial::Save(const wstring& path)
{
	return E_NOTIMPL;
}

HRESULT RMaterial::Load(const wstring& path)
{
	return E_NOTIMPL;
}

void RMaterial::Serialize(json& jsonObject) const
{
    RResource::Serialize(jsonObject);
    jsonObject["RenderingMode"] = static_cast<int>(mMode);
    
    wstring_convert<codecvt_utf8<wchar_t>, wchar_t> converter;
    jsonObject["AlbedoData"] = converter.to_bytes(mData.albedo);

    if (mAlbedoTexture)
    {
        jsonObject["AlbedoTextureName"] = converter.to_bytes(mAlbedoTexture->GetName());
    }
    else
    {
        jsonObject["AlbedoTextureName"] = "";
    }

    if (mShader)
    {
        jsonObject["ShaderName"] = converter.to_bytes(mShader->GetName());
    }
    else
    {
        jsonObject["ShaderName"] = "";
    }
}

void RMaterial::Deserialize(const json& jsonObject)
{
    RResource::Deserialize(jsonObject);
    mMode = static_cast<ERenderingMode>(jsonObject["RenderingMode"]);

    wstring_convert<codecvt_utf8<wchar_t>, wchar_t> converter;
    mData.albedo = converter.from_bytes(jsonObject["AlbedoData"]);

    wstring albedoTextureName = converter.from_bytes(jsonObject["AlbedoTextureName"]);
    if (!albedoTextureName.empty())
    {
        mAlbedoTexture = RResources::Find<RTexture>(albedoTextureName);
    }

    wstring shaderName = converter.from_bytes(jsonObject["ShaderName"]);
    if (!shaderName.empty())
    {
        mShader = RResources::Find<RShader>(shaderName);
    }
}

void RMaterial::Bind()
{
	BindShader();
	BindTextures();
}

void RMaterial::BindShader()
{
	if (mShader)
		mShader->Bind();
}

void RMaterial::BindTextures()
{
	if (mAlbedoTexture)
		mAlbedoTexture->Bind(EShaderStage::PS, (UINT)ETextureType::Albedo);
}