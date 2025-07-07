#pragma once

//DirectXTex
#include <DirectXTex.h>
#include <DirectXTex.inl>
#include <DirectXTexEXR.h>
//Engine
#include "Resource/RResource.h"
#include "Graphics/RGraphicDevice_DX11.h"


class RTexture : public RResource
{
public:
	RTexture();
	~RTexture();

	HRESULT Save(const wstring& path) override;
	HRESULT Load(const wstring& path) override;

    void Serialize(json& jsonObject) const override;
    void Deserialize(const json& jsonObject) override;

	void Bind(EShaderStage stage, UINT startSlot);

private:
	ScratchImage mImage;

	D3D11_TEXTURE2D_DESC mDesc;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> mTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mSRV;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> mRTV;
};