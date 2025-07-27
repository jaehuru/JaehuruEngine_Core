#include "Layer/JLayer.h"
#include "Collision/JCollisionManager.h"
#include "Actor/AActor.h"


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

void JLayer::EndOfFrame()
{
	vector<AActor*> deleteObjects = {};
	findDeadActors(deleteObjects);
	eraseDeadActor();
	deleteActors(deleteObjects);
}

void JLayer::Serialize(json& jsonObject) const
{
    IEntity::Serialize(jsonObject);
    json actorsArray = json::array();
    for (AActor* actor : mActors)
    {
        if (actor == nullptr)
            continue;

        json actorJson;
        actor->Serialize(actorJson);
        actorsArray.push_back(actorJson);
    }
    jsonObject["Actors"] = actorsArray;
}

void JLayer::Deserialize(const json& jsonObject)
{
    IEntity::Deserialize(jsonObject);

    // Clear existing actors
    for (AActor* actor : mActors)
    {
        if (actor != nullptr)
        {
            delete actor;
            actor = nullptr;
        }
    }
    mActors.clear();

    const json& actorsArray = jsonObject["Actors"];
    for (const auto& actorJson : actorsArray)
    {
        AActor* newActor = new AActor();
        newActor->Deserialize(actorJson);
        AddActor(newActor);
    }
}

void JLayer::AddActor(AActor* actor)
{
	if (actor == nullptr)
		return;

	mActors.push_back(actor);
}

void JLayer::EraseActor(AActor* eraseActor)
{
	erase_if(mActors,
		[=](AActor* actor)
		{
			return actor == eraseActor;
		});
}

void JLayer::findDeadActors(OUT vector<AActor*>& actors)
{
	for (AActor* actor : mActors)
	{
		AActor::EState active = actor->GetState();
		if (active == AActor::EState::Destroyed)
			actors.push_back(actor);
	}
}

void JLayer::deleteActors(vector<AActor*> actors)
{
	for (AActor* actor : actors)
	{
		delete actor;
		actor = nullptr;
	}
}

void JLayer::eraseDeadActor()
{
	erase_if(mActors,
		[](AActor* actor)
		{
			return (actor)->IsDead();
		});
}
