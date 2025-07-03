#pragma once

#include "Resource/huruResource.h"
#include "FMOD/huruFmod.h"

namespace huru
{
	class AudioClip : public Resource
	{
	public:
		AudioClip();
		virtual ~AudioClip();

		virtual HRESULT		Load(const wstring& path) override;

		void	Play();
		void	Stop();
		void	Set3DAttributes(const Vector3 pos);

		void	SetLoop(bool loop)	{ mbLoop = loop; }

	private:
		FMOD::Sound*		mSound;
		FMOD::Channel*		mChannel;
		float				mMinDistance;
		float				mMaxDistance;
		bool				mbLoop;
	};
}

