#pragma once

#include "Resource/RResource.h"
#include "Graphics/RGraphicDevice_DX11.h"

class RShader : public RResource
{
	public:
		RShader();
		virtual ~RShader();

		HRESULT Save(const wstring& path) override;
		HRESULT Load(const wstring& path) override;
		HRESULT Load(const map<EShaderStage, wstring>& shaderPaths);

        void	Serialize(json& jsonObject) const	override;
        void	Deserialize(const json& jsonObject) override;

		bool	Create(const EShaderStage stage, const wstring& fullPath);
		bool	CreateVertexShader(const wstring& fullPath);
		bool	CreatePixelShader(const wstring& fullPath);

		void	Bind();

		Microsoft::WRL::ComPtr<ID3DBlob> GetVSBlob() const { return mVSBlob; }
		Microsoft::WRL::ComPtr<ID3DBlob> GetHSBlob() const { return mHSBlob; }
		Microsoft::WRL::ComPtr<ID3DBlob> GetDSBlob() const { return mDSBlob; }
		Microsoft::WRL::ComPtr<ID3DBlob> GetGSBlob() const { return mGSBlob; }
		Microsoft::WRL::ComPtr<ID3DBlob> GetPSBlob() const { return mPSBlob; }

		Microsoft::WRL::ComPtr<ID3D11VertexShader>	GetVS() const	{ return mVS; }
		Microsoft::WRL::ComPtr<ID3D11HullShader>		GetHS() const	{ return mHS; }
		Microsoft::WRL::ComPtr<ID3D11DomainShader>	GetDS() const	{ return mDS; }
		Microsoft::WRL::ComPtr<ID3D11GeometryShader>	GetGS() const	{ return mGS; }
		Microsoft::WRL::ComPtr<ID3D11PixelShader>		GetPS() const	{ return mPS; }

		void SetRasterizerState(const ERasterizerState State)		{ mRasterizerState = State; }
		void SetBlendState(const EBlendState State)				{ mBlendState = State; }
		void SetDepthStencilState(const EDepthStencilState State)	{ mDepthStencilState = State; }

	private:
		static bool bWireframe;

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

		ERasterizerState			mRasterizerState;
		EBlendState					mBlendState;
		EDepthStencilState			mDepthStencilState;

        map<EShaderStage, wstring>	mShaderPaths;
};

