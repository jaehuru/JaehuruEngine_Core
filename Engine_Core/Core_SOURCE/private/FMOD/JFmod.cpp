#include "FMOD/JFMOD.h"



FMOD::Studio::System* JFmod::mSystem		= nullptr;
FMOD::System* JFmod::mCoreSystem			= nullptr;

void JFmod::Initialize()
{
	void* extraDriverData = nullptr;

	FMOD::Studio::System::create(&mSystem);

	mSystem->getCoreSystem(&mCoreSystem);
	mCoreSystem->setSoftwareFormat(0, FMOD_SPEAKERMODE_5POINT1, 0);

	mSystem->initialize(1024, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, extraDriverData);
}

bool JFmod::CreateSound(const string& path, FMOD::Sound** sound)
{
	if (FMOD_OK != mCoreSystem->createSound(path.c_str(), FMOD_3D, 0, sound))
		return false;

	return true;
}

void JFmod::SoundPlay(FMOD::Sound* sound, FMOD::Channel** channel)
{
	mCoreSystem->playSound(sound, 0, false, channel);
}

void JFmod::Set3DListenerAttributes(const FVector3& pos)
{
	//-1 ~ 1
	FMOD_VECTOR fmodPos(pos.x, pos.y, pos.z);
	FMOD_VECTOR fmodVel(0.0f, 0.0f, 0.0f);
	FMOD_VECTOR fmodForward(0.0f, 0.0f, 1.0f);
	FMOD_VECTOR fmodUp(0.0f, 1.0f, 0.0f);

	mCoreSystem->set3DListenerAttributes(0, &fmodPos, &fmodVel, &fmodForward, &fmodUp);
}

void JFmod::Release()
{
	mSystem->release();
	mSystem = nullptr;
}
