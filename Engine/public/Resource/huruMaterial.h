#pragma once

#include "Resource/huruResource.h"

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

		void	SetShader(Shader* shader)	{ mShader = shader; }

	private:
		eRenderingMode	mMode;
		Material::Data	mData;

		Shader*			mShader;
	};
}

