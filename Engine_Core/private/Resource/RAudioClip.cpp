#include "Resource/RAudioClip.h"


RAudioClip::RAudioClip() :
	RResource(EResourceType::RAudioClip),
	mSound(nullptr),
	mChannel(nullptr), 
	mMinDistance(1.0f), 
	mMaxDistance(1000.0f), 
	mbLoop(false)
{

}

RAudioClip::~RAudioClip()
{
	mSound->release();
	mSound = nullptr;
}

HRESULT RAudioClip::Load(const wstring& path)
{
	string cPath(path.begin(), path.end());
	if (!JFmod::CreateSound(cPath, &mSound))
		return S_FALSE;

	mSound->set3DMinMaxDistance(mMinDistance, mMaxDistance);

	return S_OK;
}

void RAudioClip::Play()
{
	if (mbLoop)
		mSound->setMode(FMOD_LOOP_NORMAL);
	else
		mSound->setMode(FMOD_LOOP_OFF);

	JFmod::SoundPlay(mSound, &mChannel);
}

void RAudioClip::Stop()
{
	mChannel->stop();
}

void RAudioClip::Set3DAttributes(const FVector3 pos)
{
	FMOD_VECTOR fmodPos(pos.x, pos.y, pos.z);
	FMOD_VECTOR fmodVel(0.0f, 0.0f, 0.0f);

	mChannel->set3DAttributes(&fmodPos, &fmodVel);
}
