#include "Component/TileMapRenderer/JTileMapRenderer.h"
#include "Actor/AActor.h"
#include "Component/Transform/JTransform.h"
#include "Resource/RTexture.h"
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
