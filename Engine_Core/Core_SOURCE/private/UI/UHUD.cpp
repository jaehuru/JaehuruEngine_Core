#include "UI/UHUD.h"
#include "Resource/RTexture.h"
#include "Resource/RResources.h"

UHUD::UHUD()
	: UUIBase(EUIType::HUD)
{
}

UHUD::~UHUD()
{
}

void UHUD::OnInit()
{
		
}

void UHUD::OnActive()
{
		
}

void UHUD::OnInActive()
{
}

void UHUD::OnUpdate()
{
}

void UHUD::OnLateUpdate()
{
}

void UHUD::OnRender()
{
	
}

void UHUD::OnClear()
{
		
}

void UHUD::Serialize(json& jsonObject) const
{
	UUIBase::Serialize(jsonObject);

	if (mTexture)
	{
		wstring wstr = mTexture->GetName();
		wstring_convert<codecvt_utf8<wchar_t>> conv;
		jsonObject["texturePath"] = conv.to_bytes(wstr);
	}
	else
	{
		jsonObject["texturePath"] = "";
	}
}

void UHUD::Deserialize(const json& jsonObject)
{
	UUIBase::Deserialize(jsonObject);

	string texturePath = jsonObject.value("texturePath", "");
	if (!texturePath.empty())
	{
		wstring_convert<codecvt_utf8<wchar_t>> conv;
		wstring wKey = conv.from_bytes(texturePath);
		mTexture = RResources::Load<RTexture>(wKey, wKey);
	}
}
