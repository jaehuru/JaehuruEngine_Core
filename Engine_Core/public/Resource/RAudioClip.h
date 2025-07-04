#pragma once

#include "Resource/RResource.h"
#include "FMOD/JFmod.h"


class RAudioClip : public RResource
{
public:
	RAudioClip();
	virtual ~RAudioClip();

	HRESULT Save(const wstring& path)			override;
	HRESULT	Load(const wstring& path)			override;

	void	Serialize(json& jsonObject) const	override;
	void	Deserialize(const json& jsonObject)	override;

	void	Play();
	void	Stop();
	void	Set3DAttributes(const FVector3 pos);

	void	SetLoop(bool loop)	{ mbLoop = loop; }

private:
	FMOD::Sound*		mSound;
	FMOD::Channel*		mChannel;
	float				mMinDistance;
	float				mMaxDistance;
	bool				mbLoop;
};


