#pragma once

#include "Common/CommonInclude.h"
#include "Actor/AActor.h"
#include "Graphics/RGraphicDevice_DX11.h"


class IApplication
{
public:
	IApplication();
	~IApplication();
		

	void	Initialize(HWND hwnd, int width, int height);
	void	InitializeWindow(HWND hwnd);
	void	AdjustWindowRect(HWND hwnd, int width, int height);
	void	ReszieGraphicDevice();
	void	InitializeEtc();

	void	Run();
	void	Close();

	void	Update();
	void	LateUpdate();
	void	Render();
	void	Present();
	void	Destroy();
	void	Release();

	HWND	GetHwnd() const		{ return mHwnd; }
	UINT	GetWidth() const	{ return mWidth; }
	UINT	GetHeight() const	{ return mHeight; }

	bool	IsLoaded() const	{ return bLoaded; }
	bool	IsRunning() const	{ return bRunning; }
	bool	IsMinimized() const { return bMinimized; }

	void	IsLoaded(bool load)				{ bLoaded = load; }

	void	SetMinimized(bool minimized)	{ bMinimized = minimized; }

private:
	unique_ptr<RGraphicDevice_DX11>	mGraphicDevice;

	bool		bLoaded;
	bool		bRunning;
	bool		bMinimized;

	HWND		mHwnd;

	UINT		mWindowWidth;
	UINT		mWindowHeight;
	UINT		mWidth;
	UINT		mHeight;
	UINT		mX, mY;
};

