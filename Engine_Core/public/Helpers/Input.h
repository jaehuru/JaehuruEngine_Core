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
	Q, W, E, R, T, Y, U, I, O, P,
	A, S, D, F, G, H, J, K, L,
	Z, X, C, V, B, N, M,
	Left, Right, Up, Down,
	LButton, MButton ,RButton,
	End,
};

class Input
{
public:
	struct Key
	{
		EKeyCode	keyCode;
		EKeyState	state;
		bool		bPressed;
	};

	static void		Initialize();
	static void		Update();

	__forceinline static bool GetKeyDown(EKeyCode code) 
	{ 
		return Keys[(UINT)code].state == EKeyState::Down; 
	}

	__forceinline static bool GetKeyUp(EKeyCode code) 
	{
		return Keys[(UINT)code].state == EKeyState::Up; 
	}

	__forceinline static bool GetKey(EKeyCode code) 
	{
		return Keys[(UINT)code].state == EKeyState::Pressed; 
	}

	__forceinline static FVector2 GetMousePosition() 
	{ 
		return mMousePosition; 
	}

private:
	static void		createKeys();
	static void		updateKeys();
	static void		updateKey(Key& key);
	static bool		isKeyDown(EKeyCode code);
	static void		updateKeyDown(Key& key);
	static void		updateKeyUp(Key& key);
	static void		getMousePositionByWindow();
	static void		clearKey();


private:
	static vector<Key>	Keys;
	static FVector2		mMousePosition;
};


