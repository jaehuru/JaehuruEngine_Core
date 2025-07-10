#pragma once

#include "Event/IEvent.h"

class MouseMovedEvent : public IEvent
{
public:
	MouseMovedEvent(const float x, const float y)
		: mMouseX(x), mMouseY(y) {
	}

	float GetX() const { return mMouseX; }
	float GetY() const { return mMouseY; }

	string ToString() const override
	{
		stringstream ss;
		ss << "MouseMovedEvent: " << mMouseX << ", " << mMouseY;
		return ss.str();
	}

	EVENT_CLASS_TYPE(MouseMoved)
	EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
private:
	float mMouseX, mMouseY;
};

