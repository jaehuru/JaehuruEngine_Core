#include "Helpers/huruInput.h"
#include "HighLevelInterface/huruApplication.h"


extern Application application;

namespace huru
{
	vector<Input::Key> Input::Keys		= { };
	Vector2 huru::Input::mMousePosition = Vector2::One;

	int ASCII[(int)eKeyCode::End] =
	{
		'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P',
		'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L',
		'Z', 'X', 'C', 'V', 'B', 'N', 'M',
		VK_LEFT, VK_RIGHT, VK_UP, VK_DOWN,
		VK_LBUTTON, VK_MBUTTON ,VK_RBUTTON
	};

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
		for (size_t i = 0; i < (UINT)eKeyCode::End; ++i)
		{
			Key key;
			key.state = eKeyState::None;
			key.keyCode = (eKeyCode)i;
			key.bPressed = false;
			Keys.push_back(key);
		}
	}

	void Input::updateKeys()
	{
		for_each(Keys.begin(), Keys.end(),
			[](Key& key) -> void { updateKey(key); });		
	}

	void Input::updateKey(Key& key)
	{
		if (GetFocus())
		{
			isKeyDown(key.keyCode) ? updateKeyDown(key) : updateKeyUp(key);
			getMousePositionByWindow();
		}
		else
		{
			clearKey();
		}
	}

	bool Input::isKeyDown(eKeyCode code)
	{
		return GetAsyncKeyState(ASCII[(UINT)code]) & 0x8000;
	}

	void Input::updateKeyDown(Key& key)
	{
		if (key.bPressed == true)
			key.state = eKeyState::Pressed;
		else
			key.state = eKeyState::Down;

		key.bPressed = true;
	}

	void Input::updateKeyUp(Key& key)
	{
		if (key.bPressed == true)
			key.state = eKeyState::Up;
		else
			key.state = eKeyState::None;

		key.bPressed = false;
	}

	void Input::getMousePositionByWindow()
	{
		POINT mousePos = {  };
		GetCursorPos(&mousePos);
		ScreenToClient(application.GetHwnd(), &mousePos);

		UINT width = application.GetWidth();
		UINT height = application.GetHeight();

		mMousePosition.x = -1.0f;
		mMousePosition.y = -1.0f;

		if ((UINT)mousePos.x > 0 && (UINT)mousePos.x < width)
			mMousePosition.x = (float)mousePos.x;

		if ((UINT)mousePos.y > 0 && (UINT)mousePos.y < height)
			mMousePosition.y = (float)mousePos.y;
	}

	void Input::clearKey()
	{
		for (Key& key : Keys)
		{
			if (key.state == eKeyState::Down ||
				key.state == eKeyState::Pressed)
				key.state = eKeyState::Up;
			else if (key.state == eKeyState::Up)
				key.state = eKeyState::None;

			key.bPressed = false;
		}
	}
}
