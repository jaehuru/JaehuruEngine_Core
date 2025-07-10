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
	T* actor = new T();
	actor->SetLayerType(type);
	JScene* activeScene = JSceneManager::GetActiveScene();
	JLayer* layer = activeScene->GetLayer(type);
	layer->AddActor(actor);

	return actor;
}

template<typename T>
static T* Instantiate(ELayerType type, FVector3 position)
{
	T* actor = new T();
	actor->SetLayerType(type);
	JScene* activeScene = JSceneManager::GetActiveScene();
	JLayer* layer = activeScene->GetLayer(type);
	layer->AddActor(actor);

	JTransform* tr = actor->GetComponent<JTransform>();
	tr->SetPosition(position);

	return actor;
}

static void JDontDestroyOnLoad(AActor* actor)
{
	JScene* activeScene = JSceneManager::GetActiveScene();
	activeScene->EraseActor(actor);

	JScene* dontDestroyOnLoad = 
		JSceneManager::GetDontDestroyOnLoad();
	dontDestroyOnLoad->
		AddActor(actor, actor->GetLayerType());
}
