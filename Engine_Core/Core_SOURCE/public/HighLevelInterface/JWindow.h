#pragma once

#include "Common/CommonInclude.h"
#include "Event/IEvent.h"


struct FWindowProps
{
	wstring Title;
	UINT Width;
	UINT Height;
	HWND Hwnd;

	FWindowProps(const wstring& title = L"Jaehuru Engine",
		UINT width = 1600,
		UINT height = 900)
		: Title(title), Width(1600), Height(900), Hwnd(NULL)
	{
	}
};

class JWindow
{
public:
	struct FWindowData
	{
		string Title;
		HWND Hwnd;

		//client
		unsigned int Width, Height;
		// window size
		unsigned int WindowWidth, WindowHeight;
		// window position
		unsigned int X, Y;

		bool VSync;
		EventCallbackFn EventCallback;
	};

	void	Initialize();
	void	SetWindowResize(UINT width, UINT height);
	
	HWND	GetHwnd() 					{ return mData.Hwnd; }
	UINT	GetWidth() 					{ return mData.Width; }
	UINT	GetHeight()					{ return mData.Height; }
	UINT	GetWindowWidth() 			{ return mData.WindowWidth; }
	UINT	GetWindowHeight() 			{ return mData.WindowHeight; }
	UINT	GetXPos() 					{ return mData.X; }
	UINT	GetYPos() 					{ return mData.Y; }

	void	SetHwnd(HWND hwnd) 									{ mData.Hwnd = hwnd; }
	void	SetWidth(UINT width) 								{ mData.Width = width; }
	void	SetHeight(UINT height) 								{ mData.Height = height; }
	void	SetWindowWidth(UINT width) 							{ mData.WindowWidth = width; }
	void	SetEventCallBack(const EventCallbackFn& callback)	{ mData.EventCallback = callback; }
	void	SetWindowHeight(UINT height)						{ mData.WindowHeight = height; }
	void	SetPos(UINT x, UINT y)								{ mData.X = x; mData.Y = y; }

private:
	FWindowData mData;
};

