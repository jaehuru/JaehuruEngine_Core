#pragma once

#include "Resource/huruResource.h"
#include "Resource/huruTexture.h"

namespace huru
{
	namespace graphics
	{
		class Shader;
	}

	class Material : public Resource
	{
	public:
		struct Data
		{
			wstring albedo;
		};

		Material();
		virtual ~Material();

		virtual HRESULT Save(const wstring& path) override;
		virtual HRESULT Load(const wstring& path) override;

		void	Bind();
		void	BindShader();
		void	BindTextures();

		void	SetShader(Shader* shader)	{ mShader = shader; }
		void	SetAlbedoTexture(Texture* texture) { mAlbedoTexture = texture; mData.albedo = texture->GetName(); }

	private:
		eRenderingMode	mMode;
		Material::Data	mData;

		Texture*		mAlbedoTexture;
		Shader*			mShader;
	};
}

