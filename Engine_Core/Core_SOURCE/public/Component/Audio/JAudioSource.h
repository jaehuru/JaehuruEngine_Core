#pragma once

#include "Component/JComponent.h"


class RAudioClip;

class JAudioSource : public JComponent
{
public:
	JAudioSource();
	~JAudioSource();

	void	Initialize()	override;
	void	Update()		override;
	void	LateUpdate()	override;
	void	Render()		override;

    void	Serialize(json& jsonObject) const	override;
    void	Deserialize(const json& jsonObject) override;

	void	Play();
	void	Stop();
	void	SetLoop(bool loop);

	RAudioClip*	GetClip() const				{ return mAudioClip; }
	void		SetClip(RAudioClip* clip)	{ mAudioClip = clip; }

private:
	RAudioClip*		mAudioClip;
};