#pragma once

#include "Event/IEvent.h"
#include "Actor/AActor.h"

class JScene;

class ActorEvent : public IEvent
{
public:
	AActor::EState GetState() const { return mState; }
	EVENT_CLASS_CATEGORY(EventCategoryActor | EventCategoryGame)

protected:
	ActorEvent(const AActor::EState state) :
		mState(state) 
	{

	}

	AActor::EState mState;
};

class ActorCreatedEvent : public ActorEvent
{
public:
	ActorCreatedEvent(AActor* actor, JScene* scene) :
		ActorEvent(AActor::EState::Created),
		mActor(actor),
		mScene(scene)
	{

	}

	string ToString() const override
	{
		stringstream ss;
		ss << "ActorCreatedEvent";
		return ss.str();
	}

	AActor* GetActor() const { return mActor; }
	JScene* GetScene() const { return mScene; }

	EVENT_CLASS_TYPE(ActorCreated)

private:
	AActor* mActor;
	JScene* mScene;
};

class ActorDestroyedEvent : public ActorEvent
{
public:
	ActorDestroyedEvent(AActor* actor, JScene* scene) :
		ActorEvent(AActor::EState::Destroyed),
		mActor(actor),
		mScene(scene)
	{

	}

	string ToString() const override
	{
		stringstream ss;
		ss << "ActorDestroyedEvent";
		return ss.str();
	}

	AActor* GetActor() const { return mActor; }
	JScene* GetScene() const { return mScene; }

	EVENT_CLASS_TYPE(ActorDestroyed)

private:
	AActor* mActor;
	JScene* mScene;
};