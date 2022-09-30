#pragma once

#include <sstream>

#include "Event.hpp"
#include "Core/KeyCode.hpp"

namespace ember {

	class KeyEvent : public Event
	{
	public:
		inline KeyCode GetKeyCode() const { return keyCode_; }

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput);
	protected:
		KeyEvent(KeyCode keycode)
			: keyCode_(keycode) {}

		KeyCode keyCode_;
	};

	class KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(KeyCode keycode)
			: KeyEvent(keycode) {}


		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << (int)keyCode_;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed);
	};

	class KeyRepeatEvent : public KeyEvent
	{
	public:
		KeyRepeatEvent(KeyCode keycode)
			: KeyEvent(keycode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyRepeatEvent: " << (int)keyCode_;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyRepeat);
	};

	class KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(KeyCode keycode)
			: KeyEvent(keycode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << (int)keyCode_;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)
	};

}