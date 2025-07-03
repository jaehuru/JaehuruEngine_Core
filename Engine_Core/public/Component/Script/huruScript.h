#pragma once

#include "Component/huruComponent.h"

namespace huru
{
	class Collider;

	class Script : public Component
	{
	public:
		Script();
		~Script();

		void	Initialize()	override;
		void	Update()		override;
		void	LateUpdate()	override;
		void	Render()		override;

		virtual void	OnCollisionEnter(Collider* other);
		virtual void	OnCollisionStay(Collider* other);
		virtual void	OnCollisionExit(Collider* other);

	private:

	};
}

