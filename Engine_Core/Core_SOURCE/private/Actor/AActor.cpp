#include "Actor/AActor.h"
#include "Helpers/Input.h"
#include "Helpers/Time.h"
#include "Component/Transform/JTransform.h"
#include "Component/TileMapRenderer/JTileMapRenderer.h"
#include "Component/Collider/JCollider.h"
#include "Component/Collider/JBoxCollider2D.h"
#include "Component/Collider/JCircleCollider2D.h"
#include "Component/Rigidbody/JRigidbody.h"
#include "Component/Script/JScript.h"
#include "Component/SpriteRenderer/JSpriteRenderer.h"
#include "Component/Animator/JAnimator.h"
#include "Component/Camera/JCamera.h"
#include "Component/Audio/JAudioListener.h"
#include "Component/Audio/JAudioSource.h"


AActor::AActor() :
	mState(EState::Active),
	mLayerType(ELayerType::None)
{
	mComponents.resize((UINT)EComponentType::End);
	initializeTransform();
}

AActor::~AActor()
{
	for (JComponent* comp : mComponents)
	{
		if (comp == nullptr)
			continue;

		delete comp;
		comp = nullptr;
	}
}

void AActor::Initialize()
{
	for (JComponent* comp : mComponents)
	{
		if (comp == nullptr)
			continue;

		comp->Initialize();
	}
}

void AActor::Update()
{
	const float speed = 100.f;
	for (JComponent* comp : mComponents)
	{
		if (comp == nullptr)
			continue;

		comp->Update();
	}
}

void AActor::LateUpdate()
{
	for (JComponent* comp : mComponents)
	{
		if (comp == nullptr)
			continue;

		comp->LateUpdate();
	}
}

void AActor::Render()
{
	for (JComponent* comp : mComponents)
	{
		if (comp == nullptr)
			continue;

		comp->Render();
	}
}

void AActor::Serialize(json& jsonObject) const
{
    IEntity::Serialize(jsonObject);
    jsonObject["State"] = static_cast<int>(mState);
    jsonObject["LayerType"] = static_cast<int>(mLayerType);

    json componentsArray = json::array();
    for (JComponent* comp : mComponents)
    {
        if (comp == nullptr)
            continue;

        json componentJson;
        comp->Serialize(componentJson);
        componentsArray.push_back(componentJson);
    }
    jsonObject["Components"] = componentsArray;
}

void AActor::Deserialize(const json& jsonObject)
{
    IEntity::Deserialize(jsonObject);
    mState = static_cast<EState>(jsonObject["State"]);
    mLayerType = static_cast<ELayerType>(jsonObject["LayerType"]);

    for (JComponent* comp : mComponents)
    {
        if (comp != nullptr)
        {
            delete comp;
            comp = nullptr;
        }
    }
    mComponents.clear();
    mComponents.resize((UINT)EComponentType::End);

    const json& componentsArray = jsonObject["Components"];
    for (const auto& componentJson : componentsArray)
    {
        EComponentType type = static_cast<EComponentType>(componentJson["Type"]);
        JComponent* newComp = nullptr;

        switch (type)
        {
        case EComponentType::JTransform:
            newComp = new JTransform();
            break;
        case EComponentType::JTileMapRenderer:
            newComp = new JTileMapRenderer();
            break;
        case EComponentType::JCollider:
        {
            EColliderType colliderType = static_cast<EColliderType>(componentJson["ColliderType"]);
            switch (colliderType)
            {
            case EColliderType::Rect2D:
                newComp = new JBoxCollider2D();
                break;
            case EColliderType::Circle2D:
                newComp = new JCircleCollider2D();
                break;
            default:
                break;
            }
        }
            break;
        case EComponentType::JRigidbody:
            newComp = new JRigidbody();
            break;
        case EComponentType::JScript:
            newComp = new JScript();
            break;
        case EComponentType::JSpriteRenderer:
            newComp = new JSpriteRenderer();
            break;
        case EComponentType::JAnimator:
            newComp = new JAnimator();
            break;
        case EComponentType::JCamera:
            newComp = new JCamera();
            break;
        case EComponentType::JAudioListener:
            newComp = new JAudioListener();
            break;
        case EComponentType::JAudioSource:
            newComp = new JAudioSource();
            break;
        default:
            break;
        }

        if (newComp != nullptr)
        {
            newComp->SetOwner(this);
            newComp->Deserialize(componentJson);
            mComponents[(UINT)type] = newComp;
        }
    }
}

void AActor::initializeTransform()
{
	AddComponent<JTransform>();
}

