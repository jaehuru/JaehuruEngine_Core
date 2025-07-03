#pragma once

#include "Component/Collider/huruCollider.h"

namespace huru
{
	class CircleCollider2D : public Collider
	{
	public:
		CircleCollider2D();
		~CircleCollider2D();

		void	Initialize()	override;
		void	Update()		override;
		void	LateUpdate()	override;
		void	Render()		override;

	private:
		Vector2 mRadius;
	};
}

