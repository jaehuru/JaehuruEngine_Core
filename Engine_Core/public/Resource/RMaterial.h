#pragma once

#include "Resource/RResource.h"
#include "Resource/RTexture.h"


class RShader;


class RMaterial : public RResource
{
public:
	struct Data
	{
		wstring albedo;
	};

	RMaterial();
	virtual ~RMaterial();

	virtual HRESULT Save(const wstring& path) override;
	virtual HRESULT Load(const wstring& path) override;

	void	Bind();
	void	BindShader();
	void	BindTextures();

	void	SetShader(RShader* shader)	{ mShader = shader; }
	void	SetAlbedoTexture(RTexture* texture) { mAlbedoTexture = texture; mData.albedo = texture->GetName(); }

private:
	ERenderingMode	mMode;
	RMaterial::Data	mData;

	RTexture*		mAlbedoTexture;
	RShader*			mShader;
};


