#pragma once

#include "Common/huruEntity.h"

namespace huru
{
	class GameObject;

	class Component : public Entity
	{
	public:
		Component(eComponentType type);
		virtual ~Component();

		virtual void	Initialize();
		virtual void	Update();
		virtual void	LateUpdate();
		virtual void	Render();

		GameObject*		GetOwner() const			{ return mOwner; }
		eComponentType	GetType() const				{ return mType; }

		void			SetOwner(GameObject* owner) { mOwner = owner; }

	private:
		GameObject*			mOwner;
		eComponentType		mType;
	};
}