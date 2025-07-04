#pragma once

#include "UUIBase.h"

class RTexture; 

class UHUD : public UUIBase
{
public:
	UHUD();
	~UHUD();

	void	OnInit()		override;
	void	OnActive()		override;
	void	OnInActive()	override;
	void	OnUpdate()		override;
	void	OnLateUpdate()	override;
	void	OnRender()		override;
	void	OnClear()		override;

	void	Serialize(json& jsonObject) const	override;
	void	Deserialize(const json& jsonObject) override;

private:
	RTexture*		mTexture;
};


