#pragma once

#include "Component/JComponent.h"
#include "Resource/RAnimation.h"



class JAnimator : public JComponent
{
public:
	struct FEvent
	{
		void operator=(function<void()> func)
		{
			mEvent = move(func);
		}

		void operator()()
		{
			if (mEvent)
				mEvent();
		}

		function<void()> mEvent;
	};

	struct Events
	{
		FEvent startEvent;
		FEvent completeEvent;
		FEvent endEvent;
	};

	JAnimator();
	~JAnimator();

	void		Initialize()	override;
	void		Update()		override;
	void		LateUpdate()	override;
	void		Render()		override;

    void		Serialize(json& jsonObject) const	override;
    void		Deserialize(const json& jsonObject) override;

	void		CreateAnimation(const wstring& name,
								RTexture* spriteSheet,
								FVector2 leftTop,
								FVector2 size,
								FVector2 offset,
								UINT spriteLength,
								float duration);
	void		CreateAnimationByFolder();

	void		AddAnimation(const wstring& name, RAnimation* animation);
	RAnimation*	FindAnimation(const wstring& name);
	void		PlayAnimation(const wstring& name, bool loop);

	Events*		FindEvents(const wstring& name);

	function<void()>&	GetStartEvent(const wstring& name);
	function<void()>&	GetCompleteEvent(const wstring& name);
	function<void()>&	GetEndEvent(const wstring& name);

	bool				IsComplete()	{ return mActiveAnimation->IsComplete(); }

private:
	map<wstring, RAnimation*>			mAnimations;
	RAnimation*							mActiveAnimation;
	bool								mbLoop;

	//FEvent
	map<wstring, Events*> mQueue;

};