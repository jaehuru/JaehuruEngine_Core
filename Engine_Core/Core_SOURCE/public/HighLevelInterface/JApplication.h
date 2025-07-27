#pragma once

#include "Common/CommonInclude.h"
#include "Actor/AActor.h"
#include "Graphics/RGraphicDevice_DX11.h"
#include "Event/IEvent.h"
#include "HighLevelInterface/JWindow.h"
#include "Event/FApplicationEvent.h"
#include "Event/FMouseEvent.h"
#include "Event/FKeyEvent.h"
#include "Event/JEventQueue.h"

class JApplication
{
public:
	JApplication();
	~JApplication();
		

	void	Initialize(HWND hwnd, int width, int height);
	void	InitializeWindow(HWND hwnd);
	void	AdjustWindowRect(HWND hwnd, int width, int height);
	void	ReszieGraphicDevice(WindowResizeEvent& e);
	void	InitializeEtc();
	void	InitializeEventHandlers();

	void	OnWindowEvent(IEvent& e);

	void	Run();
	void	Close();

	void	Update();
	void	LateUpdate();
	void	Render();
	void	Present();
	void	EndOfFrame();
	void	Release();

	void		PushEvent(IEvent* e)	{ mEventQueue.Push(e); }

	JWindow&	GetWindow() 			{ return mWindow; }
	bool		IsLoaded() const		{ return bLoaded; }
	bool		IsRunning() const		{ return bRunning; }
	bool		IsMinimized() const		{ return bMinimized; }

	void	IsLoaded(bool load)				{ bLoaded = load; }

	void	SetMinimized(bool minimized)	{ bMinimized = minimized; }

private:
	unique_ptr<RGraphicDevice_DX11>	mGraphicDevice;

	bool		bLoaded;
	bool		bRunning;
	bool		bMinimized;

	JWindow		mWindow;
	JEventQueue mEventQueue;
};

