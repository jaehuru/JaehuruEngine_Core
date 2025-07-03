#pragma once

#include "Common/CommonInclude.h"

#include "../../../External/FMOD/inc/fmod_studio.hpp"
#include "../../../External/FMOD/inc/fmod.hpp"
#include "../../../External/FMOD/inc/fmod_common.h"
#include "../../../External/FMOD/inc/fmod_codec.h"


class JFmod
{
public:
	static void		Initialize();
	static bool		CreateSound(const string& path, FMOD::Sound** sound);
	static void		SoundPlay(FMOD::Sound* sound, FMOD::Channel** channel);
	static void		Set3DListenerAttributes(const FVector3& pos);
	static void		Release();

private:
	static FMOD::Studio::System*	mSystem;
	static FMOD::System*			mCoreSystem;
};


