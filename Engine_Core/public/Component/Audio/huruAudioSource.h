#pragma once

#include "Component/huruComponent.h"

namespace huru
{
	class AudioClip;

	class AudioSource : public Component
	{
	public:
		AudioSource();
		~AudioSource();

		void	Initialize()	override;
		void	Update()		override;
		void	LateUpdate()	override;
		void	Render()		override;

		void	Play();
		void	Stop();
		void	SetLoop(bool loop);

		AudioClip*	GetClip() const				{ return mAudioClip; }
		void		SetClip(AudioClip* clip)	{ mAudioClip = clip; }

	private:
		AudioClip*		mAudioClip;
	};
}

