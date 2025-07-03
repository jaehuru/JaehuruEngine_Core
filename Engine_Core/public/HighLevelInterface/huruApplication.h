#pragma once

#include "Common/CommonInclude.h"
#include "GameObject/huruGameObject.h"
#include "Graphics/huruGraphicDevice_DX11.h"

namespace huru
{
	class Application
	{
	public:
		Application();
		~Application();
		

		void	Initialize(HWND hwnd, UINT width, UINT height);
		void	AdjustWindowRect(HWND hwnd, UINT width, UINT height);
		void	InitializeEtc();

		void	Run();

		void	Update();
		void	LateUpdate();
		void	Render();
		void	Destroy();
		void	Release();

		HWND	GetHwnd() const		{ return mHwnd; }
		HDC		GetHdc() const		{ return mHdc; }
		UINT	GetWidth() const	{ return mWidth; }
		UINT	GetHeight() const	{ return mHeight; }

		bool	IsLoaded() const	{ return mbLoaded; }

		void	IsLoaded(bool load)	{ mbLoaded = load; }

	private:
		bool			mbLoaded;
		unique_ptr<GraphicDevice_DX11>	mGraphicDevice;

		HWND			mHwnd;
		HDC				mHdc;

		HDC				mBackHdc;
		HBITMAP			mBackBitmap;

		UINT			mWidth;
		UINT			mHeight;
	};
}
