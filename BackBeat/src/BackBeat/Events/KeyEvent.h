#pragma once

#include "Event.h"

namespace BackBeat {

	class KeyEvent : public Event
	{
	public:
		inline int GetKeyCode() const { return m_KeyCode; }

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)

	protected:
		KeyEvent(int keycode)
			: m_KeyCode(keycode) {}

		int m_KeyCode;
	};

	class KeyEventPressed : public KeyEvent
	{
	public:
		KeyEventPressed(const int keycode, bool isRepeat = false)
			: KeyEvent(keycode), m_IsRepeat(isRepeat) {}

		bool isRepeat() const { return m_IsRepeat; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_KeyCode << " (repeat = " << m_IsRepeat << ")";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed);

	private:
		bool m_IsRepeat;
	};

	class KeyEventReleased : public KeyEvent
	{
	public:
		KeyEventReleased(const int keycode)
			: KeyEvent(keycode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased);
	};

}