#include "Component/SpriteRenderer/huruSpriteRenderer.h"
#include "Resource/huruTexture.h"
#include "Resource/huruResources.h"
#include "Resource/huruMaterial.h"
#include "Resource/huruMesh.h"
#include "Graphics/huruGraphicDevice_DX11.h"

namespace huru
{
	SpriteRenderer::SpriteRenderer() :
		Component(eComponentType::SpriteRenderer),
		mSprite(nullptr),
		mMaterial(nullptr),
		mMesh(nullptr)
	{

	}

	SpriteRenderer::~SpriteRenderer()
	{

	}

	void SpriteRenderer::Initialize()
	{
		mMesh = Resources::Find<Mesh>(L"RectMesh");
	}

	void SpriteRenderer::Update()
	{

	}

	void SpriteRenderer::LateUpdate()
	{

	}

	void SpriteRenderer::Render()
	{
		if (mMesh)
			mMesh->Bind();

		if (mMaterial)
			mMaterial->BindShader();

		if (mSprite)
			mSprite->Bind(eShaderStage::PS, (UINT)eTextureType::Albedo);

		if (mMesh)
			GetDevice()->DrawIndexed(mMesh->GetIndexCount(), 0, 0);
	}
}