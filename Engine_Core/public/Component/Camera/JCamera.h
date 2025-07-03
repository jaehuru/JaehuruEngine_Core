#pragma once

#include "Component/JComponent.h"


class AActor;

class JCamera : public JComponent
{
public:
	FVector2		CalculatePosition(FVector2 pos)		{ return pos - mDistance; }
	FVector2		CalcuateTilePosition(FVector2 pos)	{ return pos + mDistance; };

	JCamera();
	~JCamera();

	void	Initialize()		override;
	void	Update()			override;
	void	LateUpdate()		override;
	void	Render()			override;

	void	SetTarget(AActor* target)	{ mTarget = target; }

private:
	AActor*		mTarget;

	FVector2			mDistance;
	FVector2			mResolution;
	FVector2			mLookPosition;
};


