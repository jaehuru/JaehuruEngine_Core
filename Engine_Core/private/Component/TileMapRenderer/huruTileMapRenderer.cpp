#include "Component/TileMapRenderer/huruTileMapRenderer.h"
#include "GameObject/huruGameObject.h"
#include "Component/Transform/huruTransform.h"
#include "Resource/huruTexture.h"
#include "Renderer/huruRenderer.h"

namespace huru
{
	Vector2 TileMapRenderer::SelectedIndex = Vector2::One;

	TileMapRenderer::TileMapRenderer() :
		Component(eComponentType::TileMapRenderer),
		mTexture(nullptr),
		mSize(0.f, 0.f),
		mScale(0.f, 0.f),
		mIndex(0, 0)
	{

	}

	TileMapRenderer::~TileMapRenderer()
	{   

	}

	void TileMapRenderer::Initialize()
	{

	}

	void TileMapRenderer::Update()
	{

	}

	void TileMapRenderer::LateUpdate()
	{

	}

	void TileMapRenderer::Render()
	{

	}
}