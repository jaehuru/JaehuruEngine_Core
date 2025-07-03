#include "Resource/RTexture.h"
#include "HighLevelInterface/JApplication.h"
#include "Resource/RResources.h"

extern JApplication application;


RTexture::RTexture() :
	RResource(EResourceType::RTexture),
	mDesc{}
{

}

RTexture::~RTexture()
{

}

HRESULT RTexture::Save(const wstring& path)
{
	return E_NOTIMPL;
}

HRESULT RTexture::Load(const wstring& path)
{
	wstring ext = path.substr(path.find_last_of(L".") + 1);
		
	if (ext == L".dds" || ext == L".DDS")
	{
		if (FAILED(LoadFromDDSFile(path.c_str(), DDS_FLAGS::DDS_FLAGS_NONE, nullptr, mImage)))
			return S_FALSE;
	}
	else if (ext == L".tga" || ext == L".TGA")
	{
		if (FAILED(LoadFromTGAFile(path.c_str(), nullptr, mImage)))
			return S_FALSE;
	}
	else // WIC (png, jpg, jpeg, bmp )
	{
		if (FAILED(LoadFromWICFile(path.c_str(), WIC_FLAGS::WIC_FLAGS_NONE, nullptr, mImage)))
			return S_FALSE;
	}

	HRESULT hr = CreateShaderResourceView
	(
		GetDevice()->GetID3D11Device().Get()
		, mImage.GetImages()
		, mImage.GetImageCount()
		, mImage.GetMetadata()
		, mSRV.GetAddressOf()
	);

	if (hr == S_FALSE)
		assert(false/*"Textrue load fail!!"*/);

	mSRV->GetResource((ID3D11Resource**)mTexture.GetAddressOf());

	return S_OK;
}

void RTexture::Bind(EShaderStage stage, UINT startSlot)
{
	GetDevice()->SetShaderResource(stage, startSlot, mSRV.GetAddressOf());
}
