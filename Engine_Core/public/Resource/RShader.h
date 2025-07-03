#pragma once

#include "Resource/RResource.h"
#include "Graphics/RGraphicDevice_DX11.h"

class RShader : public RResource
{
	public:
		RShader();
		virtual ~RShader();

		virtual HRESULT Save(const wstring& path) override;
		virtual HRESULT Load(const wstring& path) override;
		HRESULT Load(const map<EShaderStage, wstring>& shaderPaths);

		bool	Create(const EShaderStage stage, const wstring& fullPath);
		bool	CreateVertexShader(const wstring& fullPath);
		bool	CreatePixelShader(const wstring& fullPath);

		void	Bind();

		Microsoft::WRL::ComPtr<ID3DBlob> GetVSBlob() { return mVSBlob; }
		Microsoft::WRL::ComPtr<ID3DBlob> GetHSBlob() { return mHSBlob; }
		Microsoft::WRL::ComPtr<ID3DBlob> GetDSBlob() { return mDSBlob; }
		Microsoft::WRL::ComPtr<ID3DBlob> GetGSBlob() { return mGSBlob; }
		Microsoft::WRL::ComPtr<ID3DBlob> GetPSBlob() { return mPSBlob; }

		void	SetRasterizerState(const ERasterizerState state)		{ mRasterizerState = state; }
		void	SetBlendState(const EBlendState state)					{ mBlendState = state; }
		void	SetDepthStencilState(const EDepthStencilState state)	{ mDepthStencilState = state; }

	private:
		Microsoft::WRL::ComPtr<ID3DBlob>		mVSBlob;
		Microsoft::WRL::ComPtr<ID3DBlob>		mHSBlob;
		Microsoft::WRL::ComPtr<ID3DBlob>		mDSBlob;
		Microsoft::WRL::ComPtr<ID3DBlob>		mGSBlob;
		Microsoft::WRL::ComPtr<ID3DBlob>		mPSBlob;

		Microsoft::WRL::ComPtr<ID3D11VertexShader>		mVS;
		Microsoft::WRL::ComPtr<ID3D11HullShader>		mHS;
		Microsoft::WRL::ComPtr<ID3D11DomainShader>		mDS;
		Microsoft::WRL::ComPtr<ID3D11GeometryShader>	mGS;
		Microsoft::WRL::ComPtr<ID3D11PixelShader>		mPS;

		ERasterizerState		mRasterizerState;
		EBlendState				mBlendState;
		EDepthStencilState		mDepthStencilState;
};
