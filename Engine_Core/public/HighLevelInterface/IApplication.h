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

	bool	IsLoaded() const	{ return mbLoaded; }
	bool	IsRunning() const	{ return mbRunning; }
	bool	IsMinimized() const { return mbMinimized; }

	void	IsLoaded(bool load)				{ mbLoaded = load; }

	void	SetMinimized(bool minimized)	{ mbMinimized = minimized; }

private:
	unique_ptr<RGraphicDevice_DX11>	mGraphicDevice;

	bool			mbLoaded;
	bool			mbRunning;
	bool			mbMinimized;

	HWND			mHwnd;

	UINT			mWidth;
	UINT			mHeight;
};

