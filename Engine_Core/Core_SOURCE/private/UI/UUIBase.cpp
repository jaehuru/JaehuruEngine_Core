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

void UUIBase::Serialize(json& jsonObject) const
{
    IEntity::Serialize(jsonObject);
    jsonObject["PositionX"] = mPosition.x;
    jsonObject["PositionY"] = mPosition.y;
    jsonObject["SizeX"] = mSize.x;
    jsonObject["SizeY"] = mSize.y;
    jsonObject["Type"] = static_cast<int>(mType);
    jsonObject["FullScreen"] = mbFullScreen;
}

void UUIBase::Deserialize(const json& jsonObject)
{
    IEntity::Deserialize(jsonObject);
    mPosition.x = jsonObject["PositionX"];
    mPosition.y = jsonObject["PositionY"];
    mSize.x = jsonObject["SizeX"];
    mSize.y = jsonObject["SizeY"];
    mType = static_cast<EUIType>(jsonObject["Type"]);
    mbFullScreen = jsonObject["FullScreen"];
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