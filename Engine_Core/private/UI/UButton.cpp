#include "UI/UButton.h"
#include "Helpers/Input.h"


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
	int a = 0;
}
