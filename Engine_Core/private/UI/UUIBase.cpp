#include "UI/UUIBase.h"


UUIBase::UUIBase(EUIType type) :
	mType(type),
	mbMouseOn(false)
{
}

UUIBase::~UUIBase()
{
}

void UUIBase::Initialize()
{
	OnInit();
}

void UUIBase::Active()
{
	mbEnabled = true;
	OnActive();
}

void UUIBase::InActive()
{
	mbEnabled = false;
	OnInActive();
}

void UUIBase::Update()
{
	if (mbEnabled)
		OnUpdate();
}

void UUIBase::LateUpdate()
{
	if (mbEnabled)
		OnLateUpdate();
}

void UUIBase::Render()
{
	if (mbEnabled)
		OnRender();
}

void UUIBase::UIClear()
{
	OnClear();
}

void UUIBase::OnInit()
{

}

void UUIBase::OnActive()
{

}

void UUIBase::OnInActive()
{

}

void UUIBase::OnUpdate()
{

}

void UUIBase::OnLateUpdate()
{

}

void UUIBase::OnRender()
{

}

void UUIBase::OnClear()
{

}
