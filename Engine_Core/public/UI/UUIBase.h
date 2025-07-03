#pragma once

#include "Common/FEntity.h"


class UUIBase : public FEntity
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

	UUIBase(EUIType type);
	virtual ~UUIBase();

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

	EUIType		GetType() const				{ return mType; }
	FVector2		GetPos() const				{ return mPosition; }
	FVector2		GetSize() const				{ return mSize; }

	bool		IsFullScreen() const		{ return mbFullScreen; }

	void		SetType(EUIType type)		{ mType = type; }
	void		SetFullScreen(bool enable)	{ mbFullScreen = enable; }
	void		SetPos(FVector2 position)	{ mPosition = position; }
	void		SetSize(FVector2 size)		{ mSize = size; }

protected:
	FVector2		mPosition;
	FVector2		mSize;
	bool		mbMouseOn;

private:
	EUIType		mType;
	bool		mbFullScreen;
	bool		mbEnabled;
	UUIBase*		mParent;
};


