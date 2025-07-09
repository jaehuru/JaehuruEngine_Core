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

	bool	Create(UINT width, UINT height, DXGI_FORMAT format, UINT bindFlag);
	bool	CreateSRV();
	bool	CreateUAV();
	bool	CreateRTV();
	bool	CreateDSV();
	bool	CreateGpuView(UINT flag);

	void	Bind(EShaderStage stage, UINT startSlot);

    void	Serialize(json& jsonObject) const override;
    void	Deserialize(const json& jsonObject) override;

	Microsoft::WRL::ComPtr<ID3D11Texture2D>				GetTexture()	{ return mTexture; }
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>		GetRTV()		{ return mRTV; }
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>		GetDSV()		{ return mDSV; }
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	GetSRV()		{ return mSRV; }
	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView>	GetUAV()		{ return mUAV; }

	void SetUAV(Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> uav)	{ mUAV = uav; }
	void SetRTV(Microsoft::WRL::ComPtr<ID3D11RenderTargetView> rtv)		{ mRTV = rtv; }
	void SetDSV(Microsoft::WRL::ComPtr<ID3D11DepthStencilView> dsv)		{ mDSV = dsv; }
	void SetSRV(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv)	{ mSRV = srv; }

private:
	ScratchImage mImage;

	D3D11_TEXTURE2D_DESC mDesc;
	Microsoft::WRL::ComPtr<ID3D11Texture2D>				mTexture;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>      mRTV;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>      mDSV;
	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView>   mUAV;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	mSRV;
};