#pragma once

#include "Common/CommonInclude.h"

namespace huru
{
	enum class eKeyState
	{
		None,
		Pressed,
		Down,
		Up,
	};

	enum class eKeyCode
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
			eKeyCode	keyCode;
			eKeyState	state;
			bool		bPressed;
		};

		static void		Initialize();
		static void		Update();

		__forceinline static bool GetKeyDown(eKeyCode code) 
		{ 
			return Keys[(UINT)code].state == eKeyState::Down; 
		}

		__forceinline static bool GetKeyUp(eKeyCode code) 
		{
			return Keys[(UINT)code].state == eKeyState::Up; 
		}

		__forceinline static bool GetKey(eKeyCode code) 
		{
			return Keys[(UINT)code].state == eKeyState::Pressed; 
		}

		__forceinline static Vector2 GetMousePosition() 
		{ 
			return mMousePosition; 
		}

	private:
		static void		createKeys();
		static void		updateKeys();
		static void		updateKey(Key& key);
		static bool		isKeyDown(eKeyCode code);
		static void		updateKeyDown(Key& key);
		static void		updateKeyUp(Key& key);
		static void		getMousePositionByWindow();
		static void		clearKey();


	private:
		static vector<Key>	Keys;
		static Vector2		mMousePosition;
	};
}

