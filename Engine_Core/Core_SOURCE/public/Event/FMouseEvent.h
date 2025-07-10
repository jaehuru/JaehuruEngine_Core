#pragma once

#include "Event/IEvent.h"
#include "Helpers/Input.h"

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

class MouseScrolledEvent : public IEvent
{
public:
	MouseScrolledEvent(const float xOffset, const float yOffset)
		: mXOffset(xOffset), mYOffset(yOffset) {
	}

	float GetXOffset() const { return mXOffset; }
	float GetYOffset() const { return mYOffset; }

	string ToString() const override
	{
		stringstream ss;
		ss << "MouseScrolledEvent: " << GetXOffset() << ", " << GetYOffset();
		return ss.str();
	}

	EVENT_CLASS_TYPE(MouseScrolled)
	EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
private:
	float mXOffset, mYOffset;
};

class MouseButtonEvent : public IEvent
{
public:
	EMouseCode GetMouseButton() const { return mButton; }

	EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput | EventCategoryMouseButton)
protected:
	MouseButtonEvent(const EMouseCode button) : 
		mButton(button) 
	{

	}

	EMouseCode mButton;
};

class MouseButtonPressedEvent : public MouseButtonEvent
{
public:
	MouseButtonPressedEvent(const EMouseCode button) : 
		MouseButtonEvent(button) 
	{

	}

	string ToString() const override
	{
		stringstream ss;
		ss << "MouseButtonPressedEvent: " << static_cast<int>(mButton);
		return ss.str();
	}

	EVENT_CLASS_TYPE(MouseButtonPressed)
};

class MouseButtonReleasedEvent : public MouseButtonEvent
{
public:
	MouseButtonReleasedEvent(const EMouseCode button) : 
		MouseButtonEvent(button) 
	{

	}

	string ToString() const override
	{
		stringstream ss;
		ss << "MouseButtonReleasedEvent: " << static_cast<int>(mButton);
		return ss.str();
	}

	EVENT_CLASS_TYPE(MouseButtonReleased)
};

