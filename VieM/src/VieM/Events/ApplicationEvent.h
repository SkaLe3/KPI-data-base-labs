#pragma once

#include "Event.h"

namespace VieM {

	class WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() = default;

		static EventType GetStaticType() { return EventType::WindowClose; }
		virtual EventType GetEventType() const override { return GetStaticType(); }
		EVENT_TYPE(WindowClose)
	};

	class WindowResizeEvent : public Event
	{
	public: WindowResizeEvent(uint32_t width, uint32_t height)
		: m_Width(width), m_Height(height) {}

		  uint32_t GetWidth() const { return m_Width; }
		  uint32_t GetHeight() const { return m_Height; }

		  static EventType GetStaticType() { return EventType::WindowResize; }
		  virtual EventType GetEventType() const override { return GetStaticType(); }
		  EVENT_TYPE(WindowResize)
	private:
		uint32_t m_Width, m_Height;
	};
}