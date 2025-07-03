#pragma once

#include "Component/huruComponent.h"

namespace huru
{
	class AudioListener : public Component
	{
	public:
		AudioListener();
		~AudioListener();

		void	Initialize()	override;
		void	Update()		override;
		void	LateUpdate()	override;
		void	Render()		override;
	};
}

