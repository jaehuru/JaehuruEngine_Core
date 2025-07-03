#include "Component/SpriteRenderer/JSpriteRenderer.h"
#include "Resource/RTexture.h"
#include "Resource/RResources.h"
#include "Resource/RMaterial.h"
#include "Resource/RMesh.h"
#include "Graphics/RGraphicDevice_DX11.h"


JSpriteRenderer::JSpriteRenderer() :
	JComponent(EComponentType::JSpriteRenderer),
	mSprite(nullptr),
	mMaterial(nullptr),
	mMesh(nullptr)
{

}

JSpriteRenderer::~JSpriteRenderer()
{

}

void JSpriteRenderer::Initialize()
{
	mMesh = RResources::Find<RMesh>(L"RectMesh");
}

void JSpriteRenderer::Update()
{

}

void JSpriteRenderer::LateUpdate()
{

}

void JSpriteRenderer::Render()
{
	if (mMesh)
		mMesh->Bind();

	if (mMaterial)
		mMaterial->BindShader();

	if (mSprite)
		mSprite->Bind(EShaderStage::PS, (UINT)ETextureType::Sprite);

	if (mMesh)
		GetDevice()->DrawIndexed(mMesh->GetIndexCount(), 0, 0);
}
