#include "Component/Animator/JAnimator.h"
#include "Resource/RResources.h"
#include "Resource/RAnimation.h"


JAnimator::JAnimator() :
	JComponent(EComponentType::JAnimator),
	mAnimations{ },
	mActiveAnimation(nullptr),
	mbLoop(false),
	mEvents{ }
{

}

JAnimator::~JAnimator()
{
	for (auto& iter : mAnimations)
	{
		delete iter.second;
		iter.second = nullptr;
	}

	for (auto& iter : mEvents)
	{
		delete iter.second;
		iter.second = nullptr;
	}
}

void JAnimator::Initialize()
{

}

void JAnimator::Update()
{
	if (mActiveAnimation)
	{
		mActiveAnimation->Update();

		Events* events = FindEvents(mActiveAnimation->GetName());

		if (mActiveAnimation->IsComplete() == true)
		{
			if (events)
				events->completeEvent();

			if (mbLoop == true)
				mActiveAnimation->Reset();
		}
	}
}

void JAnimator::LateUpdate()
{

}

void JAnimator::Render()
{

}

void JAnimator::CreateAnimation(const wstring& name,
								RTexture* spriteSheet,
								FVector2 leftTop, FVector2 size,
								FVector2 offset, UINT spriteLength,
								float duration)
{
	RAnimation* animation = nullptr;
	animation = FindAnimation(name);
	if (animation != nullptr)
		return;

	animation = new RAnimation();
	animation->SetName(name);
	animation->CreateAnimation(name, spriteSheet, leftTop, size, 
									offset, spriteLength, duration);

	animation->SetAnimator(this);

	Events* events = new Events();
	mEvents.insert(make_pair(name, events));

	mAnimations.insert(make_pair(name, animation));
}
	
void JAnimator::CreateAnimationByFolder()
{
		
}

void JAnimator::AddAnimation(const wstring& name, RAnimation* animation)
{
	if (animation == nullptr)
		return;

	mAnimations[name] = animation;
}

RAnimation* JAnimator::FindAnimation(const wstring& name)
{
	auto iter = mAnimations.find(name);
	if (iter == mAnimations.end())
		return nullptr;

	return iter->second;
}

void JAnimator::PlayAnimation(const wstring& name, bool loop)
{
	RAnimation* animation = FindAnimation(name);
	if (animation == nullptr)
		return;
	if (mActiveAnimation)
	{
		Events* currentEvents = FindEvents(mActiveAnimation->GetName());
		if (currentEvents)
			currentEvents->endEvent();
	}

	Events* nextEvents = FindEvents(animation->GetName());
	if (nextEvents)
		nextEvents->startEvent();

	mActiveAnimation = animation;
	mActiveAnimation->Reset();
	mbLoop = loop;
}

JAnimator::Events* JAnimator::FindEvents(const wstring& name)
{
	auto iter = mEvents.find(name);
	if (iter == mEvents.end())
		return nullptr;

	return iter->second;
}

function<void()>& JAnimator::GetStartEvent(const wstring& name)
{
	Events* events = FindEvents(name);
	return events->startEvent.mEvent;
}

function<void()>& JAnimator::GetCompleteEvent(const wstring& name)
{
	Events* events = FindEvents(name);
	return events->completeEvent.mEvent;
}

function<void()>& JAnimator::GetEndEvent(const wstring& name)
{
	Events* events = FindEvents(name);
	return events->endEvent.mEvent;
}
