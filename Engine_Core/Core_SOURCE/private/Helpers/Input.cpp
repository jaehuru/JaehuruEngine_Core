#include "Helpers/Input.h"
#include "HighLevelInterface/JApplication.h"


extern JApplication application;


vector<Input::FKey> Input::Keys	= { };
FVector2 Input::mMousePosition	= FVector2::One;

void Input::Initialize()
{
	createKeys();
}

void Input::Update()
{
	updateKeys();
}

void Input::createKeys()
{
	for (int vk = 0; vk <= 0xFF; ++vk)
	{
		EKeyCode keyCode = static_cast<EKeyCode>(vk);

		FKey key;
		key.bPressed = false;
		key.State = EKeyState::None;
		key.KeyCode = keyCode;
		key.VK_KeyCode = vk;
		Keys.push_back(key);
	}
}

void Input::updateKeys()
{
	for_each(Keys.begin(), Keys.end(),
		[](FKey& key) -> void { updateKey(key); });		
}

void Input::updateKey(FKey& key)
{
	if (GetFocus())
	{
		isKeyDown(key.KeyCode) ? updateKeyDown(key) : updateKeyUp(key);
		getMousePositionByWindow();
	}
	else
	{
		clearKey();
	}
}

bool Input::isKeyDown(EKeyCode code)
{
	return GetAsyncKeyState(static_cast<int>(code)) & 0x8000;
}

void Input::updateKeyDown(FKey& key)
{
	if (key.bPressed == true)
		key.State = EKeyState::Pressed;
	else
		key.State = EKeyState::Down;

	key.bPressed = true;
}

void Input::updateKeyUp(FKey& key)
{
	if (key.bPressed == true)
		key.State = EKeyState::Up;
	else
		key.State = EKeyState::None;

	key.bPressed = false;
}

void Input::getMousePositionByWindow()
{
	POINT mousePos = {  };
	GetCursorPos(&mousePos);
	ScreenToClient(application.GetWindow().GetHwnd(), &mousePos);

	UINT width = application.GetWindow().GetWidth();
	UINT height = application.GetWindow().GetHeight();

	mMousePosition.x = -1.0f;
	mMousePosition.y = -1.0f;

	if ((UINT)mousePos.x > 0 && (UINT)mousePos.x < width)
		mMousePosition.x = (float)mousePos.x;

	if ((UINT)mousePos.y > 0 && (UINT)mousePos.y < height)
		mMousePosition.y = (float)mousePos.y;
}

void Input::clearKey()
{
	for (FKey& key : Keys)
	{
		if (key.State == EKeyState::Down ||
			key.State == EKeyState::Pressed)
			key.State = EKeyState::Up;
		else if (key.State == EKeyState::Up)
			key.State = EKeyState::None;

		key.bPressed = false;
	}
}

