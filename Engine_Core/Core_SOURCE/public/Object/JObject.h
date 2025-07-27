#pragma once

#include "Component/JComponent.h"
#include "Layer/JLayer.h"
#include "Actor/AActor.h"
#include "Scene/JSceneManager.h"
#include "Scene/JScene.h"
#include "Component/Transform/JTransform.h"
#include "HighLevelInterface/JApplication.h"
#include "Event/FActorEvent.h"


extern JApplication application;

template<typename T>
static T* Instantiate(ELayerType type)
{
	T* actor = new T();
	actor->SetLayerType(type);
	JScene* activeScene = JSceneManager::GetActiveScene();

	JSceneManager::PushEvent(new ActorCreatedEvent(actor, activeScene));

	return actor;
}

template<typename T>
static T* Instantiate(ELayerType type, FVector3 position)
{
	T* actor = new T();
	actor->SetLayerType(type);

	JTransform* tr = actor->GetComponent<JTransform>();
	tr->SetPosition(position);

	JScene* activeScene = JSceneManager::GetActiveScene();
	JSceneManager::PushEvent(new ActorCreatedEvent(actor, activeScene));

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

static void Destroy(AActor* actor)
{
	if (actor != nullptr)
		actor->death();

	JScene* activeScene = JSceneManager::GetActiveScene();
	JSceneManager::PushEvent(new ActorDestroyedEvent(actor, activeScene));
}
