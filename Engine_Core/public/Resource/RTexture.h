#pragma once

#include <DirectXTex.h>
#include <DirectXTex.inl>
#include <DirectXTexEXR.h>

#include "Resource/RResource.h"
#include "Graphics/RGraphicDevice_DX11.h"


class RTexture : public RResource
{
public:
	RTexture();
	~RTexture();

	virtual HRESULT Save(const wstring& path) override;
	virtual HRESULT Load(const wstring& path) override;

	void Bind(EShaderStage stage, UINT startSlot);

private:
	ScratchImage mImage;

	D3D11_TEXTURE2D_DESC mDesc;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> mTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mSRV;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> mRTV;
};


