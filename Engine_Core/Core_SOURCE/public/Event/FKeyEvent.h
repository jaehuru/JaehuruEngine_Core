#pragma once

#include "Event/IEvent.h"
#include "Helpers/Input.h"


class KeyEvent : public IEvent
{
public:
	EKeyCode GetKeyCode() const { return mKeyCode; }

	EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
protected:
	KeyEvent(const EKeyCode keycode)
		: mKeyCode(keycode) {
	}

	EKeyCode mKeyCode;
};

class KeyPressedEvent : public KeyEvent
{
public:
	KeyPressedEvent(const EKeyCode keycode, bool isRepeat = false)
		: KeyEvent(keycode), mIsRepeat(isRepeat) {
	}

	bool IsRepeat() const { return mIsRepeat; }

	string ToString() const override
	{
		stringstream ss;
		ss << "KeyPressedEvent: " << static_cast<int>(mKeyCode) << " (repeat = " << mIsRepeat << ")";
		return ss.str();
	}

	EVENT_CLASS_TYPE(KeyPressed)
private:
	bool mIsRepeat;
};

class KeyReleasedEvent : public KeyEvent
{
public:
	KeyReleasedEvent(const EKeyCode keycode)
		: KeyEvent(keycode) {
	}

	string ToString() const override
	{
		stringstream ss;
		ss << "KeyReleasedEvent: " << static_cast<int>(mKeyCode);
		return ss.str();
	}

	EVENT_CLASS_TYPE(KeyReleased)
};

class KeyTypedEvent : public KeyEvent
{
public:
	KeyTypedEvent(const EKeyCode keycode)
		: KeyEvent(keycode) {
	}

	string ToString() const override
	{
		stringstream ss;
		ss << "KeyTypedEvent: " << static_cast<int>(mKeyCode);
		return ss.str();
	}

	EVENT_CLASS_TYPE(KeyTyped)
};