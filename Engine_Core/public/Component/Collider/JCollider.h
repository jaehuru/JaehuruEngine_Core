#pragma once

#include "Component/JComponent.h"


class JCollider : public JComponent
{

public:
	JCollider(EColliderType type);
	~JCollider();

	void	Initialize()	override;
	void	Update()		override;
	void	LateUpdate()	override;
	void	Render()		override;

	virtual void OnCollisionEnter(JCollider* other);
	virtual void OnCollisionStay(JCollider* other);
	virtual void OnCollisionExit(JCollider* other);

	FVector2			GetOffset() const				{ return mOffset; }
	UINT32			GetID()	const					{ return mID; }
	FVector2			GetSize() const					{ return mSize; }
	EColliderType	GetColliderType() const			{ return mType; }

	void			SetOffset(FVector2 offset) 		{ mOffset = offset; }
	void			SetSize(FVector2 size) 			{ mSize = size; }

private:
	static UINT32	mCollisionID;
	UINT			mID;
	FVector2			mOffset;
	FVector2			mSize;
	EColliderType	mType;
};


