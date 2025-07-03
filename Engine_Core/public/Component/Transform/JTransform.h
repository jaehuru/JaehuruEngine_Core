#pragma once

#include "Component/JComponent.h"


class JTransform : public JComponent
{
public:
	JTransform();
	~JTransform();

	void	Initialize()	override;
	void	Update()		override;
	void	LateUpdate()	override;
	void	Render()		override;

	FVector2 GetPosition() const			{ return mPosition; }
	FVector2 GetScale() const			{ return mScale; }
	float	GetRotation() const			{ return mRotation; }

	void	SetPosition(FVector2 pos)	{ mPosition.x = pos.x; mPosition.y = pos.y; }
	void	SetScale(FVector2 scale)		{ mScale = scale; }
	void	SetRotation(float rotate)	{ mRotation = rotate; }

private:
	FVector2			mPosition;
	FVector2			mScale;
	float			mRotation;
};
