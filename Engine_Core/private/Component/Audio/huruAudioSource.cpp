#include "Component/Audio/huruAudioSource.h"
#include "Resource/huruAudioClip.h"
#include "Component/Transform/huruTransform.h"
#include "GameObject/huruGameObject.h"


namespace huru
{
	AudioSource::AudioSource() : 
		Component(eComponentType::AudioSource),
		mAudioClip(nullptr)
	{
	}

	AudioSource::~AudioSource()
	{
	}

	void AudioSource::Initialize()
	{
	}

	void AudioSource::Update()
	{
	}

	void AudioSource::LateUpdate()
	{
		Transform* transform = GetOwner()->GetComponent<Transform>();
		Vector2 pos = transform->GetPosition();
	}

	void AudioSource::Render()
	{
	}

	void AudioSource::Play()
	{
		mAudioClip->Play();
	}

	void AudioSource::Stop()
	{
		mAudioClip->Stop();
	}

	void AudioSource::SetLoop(bool loop)
	{
		mAudioClip->SetLoop(loop);
	}
}