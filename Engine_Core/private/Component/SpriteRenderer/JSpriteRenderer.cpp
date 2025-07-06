#include "Component/SpriteRenderer/JSpriteRenderer.h"
#include "Resource/RTexture.h"
#include "Resource/RResources.h"
#include "Resource/RMaterial.h"
#include "Resource/RMesh.h"
#include "Graphics/RGraphicDevice_DX11.h"
#include "Component/Transform/JTransform.h"
#include "Actor/AActor.h"


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
    JTransform* transform = GetOwner()->GetComponent<JTransform>();
    if (transform)
        transform->Bind();

	if (mMesh)
		mMesh->Bind();

	if (mMaterial)
		mMaterial->BindShader();

	if (mSprite)
		mSprite->Bind(EShaderStage::PS, (UINT)ETextureType::Sprite);

	if (mMesh)
		GetDevice()->DrawIndexed(mMesh->GetIndexCount(), 0, 0);
}

void JSpriteRenderer::Serialize(json& jsonObject) const
{
    JComponent::Serialize(jsonObject);
    wstring_convert<codecvt_utf8<wchar_t>, wchar_t> converter;

    if (mSprite)
    {
        jsonObject["SpriteName"] = converter.to_bytes(mSprite->GetName());
    }
    else
    {
        jsonObject["SpriteName"] = "";
    }

    if (mMaterial)
    {
        jsonObject["MaterialName"] = converter.to_bytes(mMaterial->GetName());
    }
    else
    {
        jsonObject["MaterialName"] = "";
    }

    if (mMesh)
    {
        jsonObject["MeshName"] = converter.to_bytes(mMesh->GetName());
    }
    else
    {
        jsonObject["MeshName"] = "";
    }
}

void JSpriteRenderer::Deserialize(const json& jsonObject)
{
    JComponent::Deserialize(jsonObject);
    wstring_convert<codecvt_utf8<wchar_t>, wchar_t> converter;

    wstring spriteName = converter.from_bytes(jsonObject["SpriteName"]);
    if (!spriteName.empty())
    {
        mSprite = RResources::Find<RTexture>(spriteName);
    }

    wstring materialName = converter.from_bytes(jsonObject["MaterialName"]);
    if (!materialName.empty())
    {
        mMaterial = RResources::Find<RMaterial>(materialName);
    }

    wstring meshName = converter.from_bytes(jsonObject["MeshName"]);
    if (!meshName.empty())
    {
        mMesh = RResources::Find<RMesh>(meshName);
    }
}
