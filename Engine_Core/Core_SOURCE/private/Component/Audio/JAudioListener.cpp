#include "Component/Audio/JAudioListener.h"
#include "Component/Transform/JTransform.h"
#include "Actor/AActor.h"
#include "FMOD/JFmod.h"


JAudioListener::JAudioListener() : 
	JComponent(EComponentType::JAudioListener)
{
}

JAudioListener::~JAudioListener()
{
}

void JAudioListener::Initialize()
{
}

void JAudioListener::Update()
{
}

void JAudioListener::LateUpdate()
{
	JTransform* tr = nullptr;
	tr = GetOwner()->GetComponent<JTransform>();

	FVector3 pos;
	JFmod::Set3DListenerAttributes(pos);
}

void JAudioListener::Render()
{
}

void JAudioListener::Serialize(json& jsonObject) const
{
    JComponent::Serialize(jsonObject);
}

void JAudioListener::Deserialize(const json& jsonObject)
{
    JComponent::Deserialize(jsonObject);
}