#include "Component/TileMapRenderer/JTileMapRenderer.h"
#include "Actor/AActor.h"
#include "Component/Transform/JTransform.h"
#include "Resource/RTexture.h"
#include "Resource/RResources.h"
#include "Renderer/RRenderer.h"


FVector2 JTileMapRenderer::SelectedIndex = FVector2::One;

JTileMapRenderer::JTileMapRenderer() :
	JComponent(EComponentType::JTileMapRenderer),
	mTexture(nullptr),
	mSize(0.f, 0.f),
	mScale(0.f, 0.f),
	mIndex(0, 0)
{

}

JTileMapRenderer::~JTileMapRenderer()
{   

}

void JTileMapRenderer::Initialize()
{

}

void JTileMapRenderer::Update()
{

}

void JTileMapRenderer::LateUpdate()
{

}

void JTileMapRenderer::Render()
{

}

void JTileMapRenderer::Serialize(json& jsonObject) const
{
    JComponent::Serialize(jsonObject);
    wstring_convert<codecvt_utf8<wchar_t>, wchar_t> converter;

    if (mTexture)
    {
        jsonObject["TextureName"] = converter.to_bytes(mTexture->GetName());
    }
    else
    {
        jsonObject["TextureName"] = "";
    }

    jsonObject["SizeX"] = mSize.x;
    jsonObject["SizeY"] = mSize.y;
    jsonObject["ScaleX"] = mScale.x;
    jsonObject["ScaleY"] = mScale.y;
    jsonObject["IndexX"] = mIndex.x;
    jsonObject["IndexY"] = mIndex.y;
}

void JTileMapRenderer::Deserialize(const json& jsonObject)
{
    JComponent::Deserialize(jsonObject);
    wstring_convert<codecvt_utf8<wchar_t>, wchar_t> converter;

    wstring textureName = converter.from_bytes(jsonObject["TextureName"]);
    if (!textureName.empty())
    {
        mTexture = RResources::Find<RTexture>(textureName);
    }

    mSize.x = jsonObject["SizeX"];
    mSize.y = jsonObject["SizeY"];
    mScale.x = jsonObject["ScaleX"];
    mScale.y = jsonObject["ScaleY"];
    mIndex.x = jsonObject["IndexX"];
    mIndex.y = jsonObject["IndexY"];
}