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

	void	Serialize(json& jsonObject) const override;
	void	Deserialize(const json& jsonObject) override;

	void	Bind();

	JTransform*		GetParent()			{ return mParent; }
	const FMatrix	GetWorldMatrix()	{ return mWorldMatrix; }

	const FVector3	GetPosition()		{ return mPosition; }
	const FVector3	GetRotation()		{ return mRotation; }
	const FVector3	GetScale()			{ return mScale; }

	const FVector3	Foward()			{ return mForward; };
	const FVector3	Right()				{ return mRight; };
	const FVector3	Up()				{ return mUp; };

	void	SetPosition(FVector3 position)			{ mPosition = position; }
	void	SetPosition(float x, float y, float z)	{ mPosition = FVector3(x, y, z); }
	void	SetRotation(FVector3 rotation)			{ mRotation = rotation; }
	void	SetRotation(float x, float y, float z)	{ mRotation = FVector3(x, y, z); }
	void	SetScale(FVector3 scale)				{ mScale = scale; }
	void	SetScale(float x, float y, float z)		{ mScale = FVector3(x, y, z); }

private:
	JTransform*		mParent;
	FMatrix			mWorldMatrix;
	
	FVector3		mPosition;
	FVector3		mRotation;
	FVector3		mScale;
	
	FVector3		mForward;
	FVector3		mRight;
	FVector3		mUp;
};
