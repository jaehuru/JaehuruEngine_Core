#include "Scene/JScene.h"
#include "Collision/JCollisionManager.h"
#include "Component/Camera/JCamera.h"
#include "Scene/JSceneManager.h"


JScene::JScene() :
	mLayers{}
{
	createLayers();
}

JScene::~JScene()
{
	for (JLayer* layer : mLayers)
	{
		if (layer == nullptr)
			continue;

		delete layer;
		layer = nullptr;
	}
}
void JScene::Initialize()
{
    const wstring& sceneName = GetName();
    JSceneManager::SetActiveScene(sceneName);
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

void JScene::Serialize(json& jsonObject) const
{
    
}

void JScene::Deserialize(const json& jsonObject)
{
    
}

void JScene::OnEnter()
{
		
}

void JScene::OnExit()
{
	JCollisionManager::Clear();
}

void JScene::AddActor(AActor* actor, const ELayerType type)
{
	mLayers[(UINT)type]->AddActor(actor);
}

void JScene::EraseActor(AActor* actor)
{
	ELayerType layerType = actor->GetLayerType();
	mLayers[(UINT)layerType]->EraseActor(actor);
}

void JScene::createLayers()
{
	mLayers.resize((UINT)ELayerType::Max);

	for_each(mLayers.begin(), mLayers.end(),
		[](JLayer*& layer) { layer = new JLayer(); });
}
