#pragma once

#include "Common/huruEntity.h"

namespace huru
{
	class UIBase : public Entity
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

		UIBase(eUIType type);
		virtual ~UIBase();

		void	Initialize();
		void	Active();
		void	InActive();
		void	Update();
		void	LateUpdate();
		void	Render();
		void	UIClear();

		virtual void	OnInit();
		virtual void	OnActive();
		virtual void	OnInActive();
		virtual void	OnUpdate();
		virtual void	OnLateUpdate();
		virtual void	OnRender();
		virtual void	OnClear();

		eUIType		GetType() const				{ return mType; }
		Vector2		GetPos() const				{ return mPosition; }
		Vector2		GetSize() const				{ return mSize; }

		bool		IsFullScreen() const		{ return mbFullScreen; }

		void		SetType(eUIType type)		{ mType = type; }
		void		SetFullScreen(bool enable)	{ mbFullScreen = enable; }
		void		SetPos(Vector2 position)	{ mPosition = position; }
		void		SetSize(Vector2 size)		{ mSize = size; }

	protected:
		Vector2		mPosition;
		Vector2		mSize;
		bool		mbMouseOn;

	private:
		eUIType		mType;
		bool		mbFullScreen;
		bool		mbEnabled;
		UIBase*		mParent;
	};
}

