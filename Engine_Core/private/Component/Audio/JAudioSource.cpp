#include "Component/Audio/JAudioSource.h"
#include "Resource/RAudioClip.h"
#include "Component/Transform/JTransform.h"
#include "Actor/AActor.h"



JAudioSource::JAudioSource() : 
	JComponent(EComponentType::JAudioSource),
	mAudioClip(nullptr)
{
}

JAudioSource::~JAudioSource()
{
}

void JAudioSource::Initialize()
{
}

void JAudioSource::Update()
{
}

void JAudioSource::LateUpdate()
{
	JTransform* transform = GetOwner()->GetComponent<JTransform>();
	FVector2 pos = transform->GetPosition();
}

void JAudioSource::Render()
{
}

void JAudioSource::Play()
{
	mAudioClip->Play();
}

void JAudioSource::Stop()
{
	mAudioClip->Stop();
}

void JAudioSource::SetLoop(bool loop)
{
	mAudioClip->SetLoop(loop);
}
