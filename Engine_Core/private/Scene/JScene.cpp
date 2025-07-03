#include "Scene/JScene.h"
#include "Collision/JCollisionManager.h"


JScene::JScene() :
	mLayers{}
{
	createLayers();
}

JScene::~JScene()
{
	for (JLayer* layer : mLayers)
	{
		delete layer;
		layer = nullptr;
	}
}
void JScene::Initialize()
{
	for (JLayer* layer : mLayers)
	{
		if (layer == nullptr)
			continue;

		layer->Initialize();
	}
}

void JScene::Update()
{
	for (JLayer* layer : mLayers)
	{
		if (layer == nullptr)
			continue;

		layer->Update();
	}
}

void JScene::LateUpdate()
{
	for (JLayer* layer : mLayers)
	{
		if (layer == nullptr)
			continue;

		layer->LateUpdate();
	}
}

void JScene::Render()
{
	for (JLayer* layer : mLayers)
	{
		if (layer == nullptr)
			continue;

		layer->Render();
	}
}

void JScene::Destroy()
{
	for (JLayer* layer : mLayers)
	{
		if (layer == nullptr)
			continue;

		layer->Destroy();
	}
}

void JScene::OnEnter()
{
		
}

void JScene::OnExit()
{
	JCollisionManager::Clear();
}

void JScene::AddGameObject(AActor* gameObj, const ELayerType type)
{
	mLayers[(UINT)type]->AddGameObject(gameObj);
}

void JScene::EraseGameObject(AActor* gameObj)
{
	ELayerType layerType = gameObj->GetLayerType();
	mLayers[(UINT)layerType]->EraseGameObject(gameObj);
}

void JScene::createLayers()
{
	mLayers.resize((UINT)ELayerType::Max);

	for_each(mLayers.begin(), mLayers.end(),
		[](JLayer*& layer) { layer = new JLayer(); });
}
