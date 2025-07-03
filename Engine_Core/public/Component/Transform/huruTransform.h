#pragma once

#include "Component/huruComponent.h"

namespace huru
{
	class Transform : public Component
	{
	public:
		Transform();
		~Transform();

		void	Initialize()	override;
		void	Update()		override;
		void	LateUpdate()	override;
		void	Render()		override;

		Vector2 GetPosition() const			{ return mPosition; }
		Vector2 GetScale() const			{ return mScale; }
		float	GetRotation() const			{ return mRotation; }

		void	SetPosition(Vector2 pos)	{ mPosition.x = pos.x; mPosition.y = pos.y; }
		void	SetScale(Vector2 scale)		{ mScale = scale; }
		void	SetRotation(float rotate)	{ mRotation = rotate; }

	private:
		Vector2			mPosition;
		Vector2			mScale;
		float			mRotation;
	};
}