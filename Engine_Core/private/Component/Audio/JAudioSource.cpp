#include "Component/Audio/JAudioSource.h"
#include "Resource/RAudioClip.h"
#include "Resource/RResources.h"
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
    FVector3 pos = transform->GetPosition();
}

void JAudioSource::Render()
{
}

void JAudioSource::Serialize(json& jsonObject) const
{
    JComponent::Serialize(jsonObject);
    wstring_convert<codecvt_utf8<wchar_t>, wchar_t> converter;

    if (mAudioClip)
    {
        jsonObject["AudioClipName"] = converter.to_bytes(mAudioClip->GetName());
    }
    else
    {
        jsonObject["AudioClipName"] = "";
    }
}

void JAudioSource::Deserialize(const json& jsonObject)
{
    JComponent::Deserialize(jsonObject);
    wstring_convert<codecvt_utf8<wchar_t>, wchar_t> converter;

    wstring audioClipName = converter.from_bytes(jsonObject["AudioClipName"]);
    if (!audioClipName.empty())
    {
        mAudioClip = RResources::Find<RAudioClip>(audioClipName);
    }
}

void JAudioSource::Play()
{
	if (mAudioClip)
		mAudioClip->Play();
}

void JAudioSource::Stop()
{
	if (mAudioClip)
		mAudioClip->Stop();
}

void JAudioSource::SetLoop(bool loop)
{
	if (mAudioClip)
		mAudioClip->SetLoop(loop);
}