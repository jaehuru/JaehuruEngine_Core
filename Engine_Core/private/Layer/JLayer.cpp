#include "Layer/JLayer.h"


JLayer::JLayer() :
	mActors{}
{

}

JLayer::~JLayer()
{
	for (AActor* actor : mActors)
	{
		if (actor == nullptr)
			continue;

		delete actor;
		actor = nullptr;
	}
}

void JLayer::Initialize()
{
	for (AActor* actor : mActors)
	{
		if (actor == nullptr)
			continue;

		actor->Initialize();
	}
}

void JLayer::Update()
{
	for (AActor* actor : mActors)
	{
		if (actor == nullptr)
			continue;

		if (actor->IsActive() == false)
			continue;

		actor->Update();
	}
}

void JLayer::LateUpdate()
{
	for (AActor* actor : mActors)
	{
		if (actor == nullptr)
			continue;

		if (actor->IsActive() == false)
			continue;

		actor->LateUpdate();
	}
}

void JLayer::Render()
{
	for (AActor* actor : mActors)
	{
		if (actor == nullptr)
			continue;

		if (actor->IsActive() == false)
			continue;

		actor->Render();
	}
}

void JLayer::Destroy()
{
	vector<AActor*> deleteObjects = {};
	findDeadGameObjects(deleteObjects);
	eraseDeadGameObject();
	deleteGameObjects(deleteObjects);
}

void JLayer::AddGameObject(AActor* actor)
{
	if (actor == nullptr)
		return;

	mActors.push_back(actor);
}

void JLayer::EraseGameObject(AActor* eraseActor)
{
	erase_if(mActors,
		[=](AActor* actor)
		{
			return actor == eraseActor;
		});
}

void JLayer::findDeadGameObjects(OUT vector<AActor*>& actors)
{
	for (AActor* actor : mActors)
	{
		AActor::EState active = actor->GetState();
		if (active == AActor::EState::Dead)
			actors.push_back(actor);
	}
}

void JLayer::deleteGameObjects(vector<AActor*> actors)
{
	for (AActor* actor : actors)
	{
		delete actor;
		actor = nullptr;
	}
}

void JLayer::eraseDeadGameObject()
{
	erase_if(mActors,
		[](AActor* actor)
		{
			return (actor)->IsDead();
		});
}
