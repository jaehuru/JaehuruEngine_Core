#include "HighLevelInterface/JWindow.h"
#include "Event/JApplicationEvent.h"
#include "Event/JMouseEvent.h"


void JWindow::Initialize()
{

}

void JWindow::SetWindowResize(UINT width, UINT height)
{
	mData.Width = width;
	mData.Height = height;

	WindowResizeEvent event(width, height);

	if (mData.EventCallback)
		mData.EventCallback(event);
}

void JWindow::SetCursorPos(double x, double y)
{
	MouseMovedEvent event(x, y);

	if (mData.EventCallback)
		mData.EventCallback(event);
}