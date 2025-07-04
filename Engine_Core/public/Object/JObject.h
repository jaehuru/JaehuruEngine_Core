#pragma once

#include "Component/JComponent.h"
#include "Layer/JLayer.h"
#include "Actor/AActor.h"
#include "Scene/JSceneManager.h"
#include "Scene/JScene.h"
#include "Component/Transform/JTransform.h"


template<typename T>
static T* Instantiate(ELayerType type)
{
	T* gameObj = new T();
	gameObj->SetLayerType(type);
	JScene* activeScene = JSceneManager::GetActiveScene();
	JLayer* layer = activeScene->GetLayer(type);
	layer->AddActor(gameObj);

	return gameObj;
}

template<typename T>
static T* Instantiate(ELayerType type, FVector2 position)
{
	T* gameObj = new T();
	gameObj->SetLayerType(type);
	JScene* activeScene = JSceneManager::GetActiveScene();
	JLayer* layer = activeScene->GetLayer(type);
	layer->AddActor(gameObj);

	JTransform* tr = gameObj->GetComponent<JTransform>();
	tr->SetPosition(position);

	return gameObj;
}

static void JDontDestroyOnLoad(AActor* actor)
{
	JScene* activeScene = JSceneManager::GetActiveScene();
	// 현재씬에서 게임 오브젝트를 삭제
	activeScene->EraseActor(actor);

	// 해당 게임 오브젝트를 -> DonDestroy씬으로 넣음
	JScene* dontDestroyOnLoad = 
		JSceneManager::GetDontDestroyOnLoad();
	dontDestroyOnLoad->
		AddActor(actor, actor->GetLayerType());
}
