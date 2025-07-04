#include "UI/UButton.h"
#include "Helpers/Input.h"
#include "Resource/RTexture.h"
#include "Resource/RResources.h"


UButton::UButton()
	: UUIBase(EUIType::Button)
{

}

UButton::~UButton()
{
}

void UButton::OnInit()
{
	SetPos(FVector2(200.0f, 200.0f));
	SetSize(FVector2(200.0f, 200.0f));

	mOnClick = bind(&UButton::ButtonClick, this);
}

void UButton::OnActive()
{
}

void UButton::OnInActive()
{
}

void UButton::OnUpdate()
{
	FVector2 mousePos = Input::GetMousePosition();

	if (mPosition.x <= mousePos.x && mousePos.x <= mPosition.x + mSize.x
		&& mPosition.y <= mousePos.y && mousePos.y <= mPosition.y + mSize.y)
	{
		mbMouseOn = true;
	}
	else
	{
		mbMouseOn = false;
	}

	if (Input::GetKeyDown(EKeyCode::LButton))
	{
		if (mbMouseOn)
		{
			mOnClick();
		}
	}
}

void UButton::OnLateUpdate()
{
}

void UButton::OnRender()
{

}

void UButton::OnClear()
{

}

void UButton::ButtonClick()
{
}

void UButton::Serialize(json& jsonObject) const
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

void UButton::Deserialize(const json& jsonObject)
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
