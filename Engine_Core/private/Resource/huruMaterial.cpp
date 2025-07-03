#include "Resource/huruMaterial.h"
#include "Resource/huruShader.h"

namespace huru
{
	Material::Material() :
		Resource(eResourceType::Material),
		mMode(eRenderingMode::Opaque),
		mAlbedoTexture(nullptr),
		mShader(nullptr)
	{

	}

	Material::~Material()
	{
	}

	HRESULT Material::Save(const wstring& path)
	{
		return E_NOTIMPL;
	}

	HRESULT Material::Load(const wstring& path)
	{
		return E_NOTIMPL;
	}

	void Material::Bind()
	{
		BindShader();
		BindTextures();
	}

	void Material::BindShader()
	{
		if (mShader)
			mShader->Bind();
	}

	void Material::BindTextures()
	{
		if (mAlbedoTexture)
			mAlbedoTexture->Bind(eShaderStage::PS, (UINT)eTextureType::Albedo);
	}
}