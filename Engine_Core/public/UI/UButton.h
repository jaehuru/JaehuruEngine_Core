#pragma once

#include "UUIBase.h"



class RTexture;


class UButton : public UUIBase
{
public:
	UButton();
	~UButton();

	virtual void	OnInit()		override;
	virtual void	OnActive()		override;
	virtual void	OnInActive()	override;
	virtual void	OnUpdate()		override;
	virtual void	OnLateUpdate()	override;
	virtual void	OnRender()		override;
	virtual void	OnClear()		override;

	virtual void	ButtonClick();

private:
	RTexture*	mTexture;
	FEvent		mOnClick;
};


