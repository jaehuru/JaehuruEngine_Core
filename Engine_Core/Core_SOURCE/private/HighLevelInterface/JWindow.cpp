#include "HighLevelInterface/JWindow.h"
#include "Event/FApplicationEvent.h"



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
