#pragma once

#include "Common/CommonInclude.h"


enum class EKeyState
{
	None,
	Pressed,
	Down,
	Up,
};

enum class EKeyCode
{
	Q = 0x51, W = 0x57, E = 0x45, R = 0x52, T = 0x54,
	Y = 0x59, U = 0x55, I = 0x49, O = 0x4F, P = 0x50,
	A = 0x41, S = 0x53, D = 0x44, F = 0x46, G = 0x47,
	H = 0x48, J = 0x4A, K = 0x4B, L = 0x4C,
	Z = 0x5A, X = 0x58, C = 0x43, V = 0x56, B = 0x42,
	N = 0x4E, M = 0x4D,

	Left = VK_LEFT, Right = VK_RIGHT, Down = VK_DOWN, Up = VK_UP,
	LButton = VK_LBUTTON, MButton = VK_MBUTTON, RButton = VK_RBUTTON,

	LeftShift = VK_LSHIFT, Leftcontrol = VK_LCONTROL, LeftAlt = VK_LMENU,
	RightShift = VK_RSHIFT, RightControl = VK_RCONTROL, RightAlt = VK_RMENU,

	End = VK_END
};

enum class EMouseCode
{
	Button0 = 0,
	Button1 = 1,
	Button2 = 2,
	Button3 = 3,
	Button4 = 4,
	Button5 = 5,
	Button6 = 6,
	Button7 = 7,

	ButtonLast = Button7,
	ButtonLeft = Button0,
	ButtonRight = Button1,
	ButtonMiddle = Button2
};

class Input
{
public:
	struct FKey
	{
		bool		bPressed;
		EKeyState	State;
		EKeyCode	KeyCode;
		int			VK_KeyCode;
	};

	static void		Initialize();
	static void		Update();

	__forceinline static bool GetKeyDown(EKeyCode code) 
	{ 
		return Keys[(UINT)code].State == EKeyState::Down; 
	}
	__forceinline static bool GetKeyUp(EKeyCode code) 
	{
		return Keys[(UINT)code].State == EKeyState::Up; 
	}
	__forceinline static bool GetKey(EKeyCode code) 
	{
		return Keys[(UINT)code].State == EKeyState::Pressed; 
	}
	__forceinline static FVector2 GetMousePosition() 
	{ 
		return mMousePosition; 
	}

private:
	static void		createKeys();
	static void		updateKeys();
	static void		updateKey(FKey& key);
	static bool		isKeyDown(EKeyCode code);
	static void		updateKeyDown(FKey& key);
	static void		updateKeyUp(FKey& key);
	static void		getMousePositionByWindow();
	static void		clearKey();

private:
	static vector<FKey>	Keys;
	static FVector2		mMousePosition;
};


