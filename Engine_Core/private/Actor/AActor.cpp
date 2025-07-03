#include "Actor/AActor.h"
#include "Helpers/Input.h"
#include "Helpers/Time.h"
#include "Component/Transform/JTransform.h"


void Destroy(AActor* actor)
{
	if (actor != nullptr)
		actor->death();
}

AActor::AActor() :
	mState(EState::Active),
	mLayerType(ELayerType::None)
{
	mComponents.resize((UINT)EComponentType::End);
	initializeTransform();
}

AActor::~AActor()
{
	for (JComponent* comp : mComponents)
	{
		if (comp == nullptr)
			continue;

		delete comp;
		comp = nullptr;
	}
}

void AActor::Initialize()
{
	for (JComponent* comp : mComponents)
	{
		if (comp == nullptr)
			continue;

		comp->Initialize();
	}
}

void AActor::Update()
{
	const float speed = 100.f;
	for (JComponent* comp : mComponents)
	{
		if (comp == nullptr)
			continue;

		comp->Update();
	}
}

void AActor::LateUpdate()
{
	for (JComponent* comp : mComponents)
	{
		if (comp == nullptr)
			continue;

		comp->LateUpdate();
	}
}

void AActor::Render()
{
	for (JComponent* comp : mComponents)
	{
		if (comp == nullptr)
			continue;

		comp->Render();
	}
}

void AActor::initializeTransform()
{
	AddComponent<JTransform>();
}
