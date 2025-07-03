#pragma once

#include "Common/FEntity.h"


class AActor;

class JComponent : public FEntity
{
public:
	JComponent(EComponentType type);
	virtual ~JComponent();

	virtual void	Initialize();
	virtual void	Update();
	virtual void	LateUpdate();
	virtual void	Render();

	AActor*		GetOwner() const			{ return mOwner; }
	EComponentType	GetType() const				{ return mType; }

	void			SetOwner(AActor* owner) { mOwner = owner; }

private:
	AActor*			mOwner;
	EComponentType		mType;
};
