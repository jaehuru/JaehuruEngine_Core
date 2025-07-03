#pragma once

#include "Component/huruComponent.h"
#include "Layer/huruLayer.h"
#include "GameObject/huruGameObject.h"
#include "Scene/huruSceneManager.h"
#include "Scene/huruScene.h"
#include "Component/Transform/huruTransform.h"

namespace huru::object
{
	template<typename T>
	static T* Instantiate(eLayerType type)
	{
		T* gameObj = new T();
		gameObj->SetLayerType(type);
		Scene* activeScene = SceneManager::GetActiveScene();
		Layer* layer = activeScene->GetLayer(type);
		layer->AddGameObject(gameObj);

		return gameObj;
	}

	template<typename T>
	static T* Instantiate(eLayerType type, Vector2 position)
	{
		T* gameObj = new T();
		gameObj->SetLayerType(type);
		Scene* activeScene = SceneManager::GetActiveScene();
		Layer* layer = activeScene->GetLayer(type);
		layer->AddGameObject(gameObj);

		Transform* tr = gameObj->GetComponent<Transform>();
		tr->SetPosition(position);

		return gameObj;
	}

	static void DontDestroyOnLoad(GameObject* gameObject)
	{
		Scene* activeScene = SceneManager::GetActiveScene();
		// 현재씬에서 게임 오브젝트를 삭제
		activeScene->EraseGameObject(gameObject);

		// 해당 게임 오브젝트를 -> DonDestroy씬으로 넣음
		Scene* dontDestroyOnLoad = 
			SceneManager::GetDontDestroyOnLoad();
		dontDestroyOnLoad->
			AddGameObject(gameObject, gameObject->GetLayerType());
	}
}