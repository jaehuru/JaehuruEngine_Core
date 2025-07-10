#pragma once

#include "Event/IEvent.h"


class WindowResizeEvent : public IEvent
{
public:
	WindowResizeEvent(unsigned int width, unsigned int height) : 
		mWidth(width),
		mHeight(height) 
	{

	}

	unsigned int GetWidth() const	{ return mWidth; }
	unsigned int GetHeight() const	{ return mHeight; }

	string ToString() const override
	{
		stringstream ss;
		ss << "WindowResizeEvent: " << mWidth << ", " << mHeight;
		return ss.str();
	}

	EVENT_CLASS_TYPE(SetWindowResize)
	EVENT_CLASS_CATEGORY(EventCategoryApplication)
private:
	unsigned int mWidth, mHeight;
};

class WindowCloseEvent : public IEvent
{
public:
	WindowCloseEvent() = default;

	EVENT_CLASS_TYPE(WindowClose)
	EVENT_CLASS_CATEGORY(EventCategoryApplication)
};

