#pragma once

#include "Component/huruComponent.h"
#include "Resource/huruAnimation.h"


namespace huru
{
	class Animator : public Component
	{
	public:
		struct Event
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
			Event startEvent;
			Event completeEvent;
			Event endEvent;
		};

		Animator();
		~Animator();

		void		Initialize()	override;
		void		Update()		override;
		void		LateUpdate()	override;
		void		Render()		override;

		void		CreateAnimation(const wstring& name,
									Texture* spriteSheet,
									Vector2 leftTop,
									Vector2 size,
									Vector2 offset,
									UINT spriteLength,
									float duration);
		void		CreateAnimationByFolder();

		void		AddAnimation(const wstring& name, Animation* animation);
		Animation*	FindAnimation(const wstring& name);
		void		PlayAnimation(const wstring& name, bool loop);

		Events*		FindEvents(const wstring& name);

		function<void()>&	GetStartEvent(const wstring& name);
		function<void()>&	GetCompleteEvent(const wstring& name);
		function<void()>&	GetEndEvent(const wstring& name);

		bool				IsComplete()	{ return mActiveAnimation->IsComplete(); }

	private:
		map<wstring, Animation*>			mAnimations;
		Animation*							mActiveAnimation;
		bool								mbLoop;

		//Event
		map<wstring, Events*>				mEvents;

	};
}

