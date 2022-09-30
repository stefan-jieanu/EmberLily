#pragma once

#include <string>
#include "Core/Defines.hpp"

namespace ember {
	// Events are currently blocking, meaning when an event occurs it
	// immediately gets dispatched and must be dealt with rigth then and there.
	// For the future, a better strategy might be to buffer events in an event
	// bus and process them during the "event" part of the update stage.

	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyRepeat, KeyReleased,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	enum EventCategory
	{
		None = 0,
		EventCategoryApplication  = BIT(0),
		EventCategoryInput		  = BIT(1),
		EventCategoryKeyboard	  = BIT(2),
		EventCategoryMouse		  = BIT(3),
		EventCategoryMouseButton  = BIT(4)
	};

	#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::##type; }\
								   virtual EventType GetEventType() const override { return GetStaticType(); }\
								   virtual const char* GetName() const override { return #type; }

	#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

	class Event
	{
		friend class EventDispatcher;

	public:
		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); }

		inline bool IsInCategory(EventCategory category)
		{
			return GetCategoryFlags() & category;
		}

		inline bool IsHandled() const
		{
			return handled_;
		}
	protected: 
		bool handled_ = false;
	};

	class EventDispatcher
	{
		template<typename T>
		using EventFn = std::function<bool(T&)>;
	public:
		EventDispatcher(Event& event)
			: event_(event) {}

		template<typename T>
		bool Dispatch(EventFn<T> func)
		{
			if (event_.GetEventType() == T::GetStaticType())
			{
				event_.handled_ = func(*(T*)&event_);
				return true;
			}
			return false;
		}
	private:
		Event& event_;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}

}