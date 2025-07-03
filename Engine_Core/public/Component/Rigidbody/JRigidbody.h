#pragma once

#include "Component/JComponent.h"


class JRigidbody : public JComponent
{
public:
	JRigidbody();
	~JRigidbody();

	void		Initialize()	override;
	void		Update()		override;
	void		LateUpdate()	override;
	void		Render()		override;

	FVector2		GetVelocity() const				{ return mVelocity; }

	void		SetMass(float mass)				{ mMass = mass; }
	void		AddForce(FVector2 force)			{ mForce = force; }
	void		SetGround(bool ground)			{ mbGround = ground; }
	void		SetVelocity(FVector2 velocity)	{ mVelocity = velocity; }

private:
	bool			mbGround;
	float			mMass;				
	float			mFriction;			
											
	FVector2			mForce;				
	FVector2			mAccelation;		
	FVector2			mVelocity;			
	FVector2			mLimitedVelocity;	
	FVector2			mGravity;			
};


