#include "Scene/JScene.h"
#include "Collision/JCollisionManager.h"
#include "Component/Camera/JCamera.h"


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

void JScene::Serialize(json& jsonObject) const
{
    FEntity::Serialize(jsonObject);
    json layersArray = json::array();
    for (JLayer* layer : mLayers)
    {
        if (layer == nullptr)
            continue;

        json layerJson;
        layer->Serialize(layerJson);
        layersArray.push_back(layerJson);
    }
    jsonObject["Layers"] = layersArray;
}

void JScene::Deserialize(const json& jsonObject)
{
    FEntity::Deserialize(jsonObject);

    for (JLayer* layer : mLayers)
    {
        if (layer != nullptr)
        {
            delete layer;
            layer = nullptr;
        }
    }
    mLayers.clear();
    mLayers.resize((UINT)ELayerType::Max);

    const json& layersArray = jsonObject["Layers"];
    for (const auto& layerJson : layersArray)
    {
        JLayer* newLayer = new JLayer();
        newLayer->Deserialize(layerJson);
        ELayerType layerType = static_cast<ELayerType>(layerJson["Type"]);
        mLayers[(UINT)layerType] = newLayer;
    }

    for (JLayer* layer : mLayers)
    {
        if (layer == nullptr)
            continue;

        for (AActor* actor : layer->GetActors())
        {
            if (actor == nullptr)
                continue;

            for (JComponent* comp : actor->GetComponents())
            {
                JCamera* camera = dynamic_cast<JCamera*>(comp);
                if (camera)
                {
                    camera->LinkTargetActor();
                }
            }
        }
    }
}

void JScene::OnEnter()
{
		
}

void JScene::OnExit()
{
	JCollisionManager::Clear();
}

void JScene::AddActor(AActor* gameObj, const ELayerType type)
{
	mLayers[(UINT)type]->AddActor(gameObj);
}

void JScene::EraseActor(AActor* gameObj)
{
	ELayerType layerType = gameObj->GetLayerType();
	mLayers[(UINT)layerType]->EraseActor(gameObj);
}

void JScene::createLayers()
{
	mLayers.resize((UINT)ELayerType::Max);

	for_each(mLayers.begin(), mLayers.end(),
		[](JLayer*& layer) { layer = new JLayer(); });
}
