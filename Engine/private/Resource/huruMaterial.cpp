#include "Resource/huruMaterial.h"
#include "Resource/huruShader.h"

namespace huru
{
	Material::Material() :
		Resource(eResourceType::Material),
		mMode(eRenderingMode::Opaque)
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
		mShader->Bind();
	}
}