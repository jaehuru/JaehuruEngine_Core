#include "Resource/RMaterial.h"
#include "Resource/RShader.h"


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
