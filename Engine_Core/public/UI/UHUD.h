#pragma once

#include "UUIBase.h"



 
class RTexture; 


class UHUD : public UUIBase
{
public:
	UHUD();
	~UHUD();

	virtual void	OnInit()		override;
	virtual void	OnActive()		override;
	virtual void	OnInActive()	override;
	virtual void	OnUpdate()		override;
	virtual void	OnLateUpdate()	override;
	virtual void	OnRender()		override;
	virtual void	OnClear()		override;

private:
	RTexture*		mTexture;
};


