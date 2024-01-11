#ifndef CODEX_EVENTS_APPLICATION_EVENT_H
#define CODEX_EVENTS_APPLICATION_EVENT_H

#include <sdafx.h>

#include "Event.h"

namespace codex::events {
    class CODEX_API WindowResizeEvent : public Event
    {
    private:
        i32 m_Width;
        i32 m_Height;

    public:
        WindowResizeEvent(const i32 width, const i32 height) : m_Width(width), m_Height(height) {}

    public:
        inline i32         GetWidth() const noexcept { return m_Width; }
        inline i32         GetHeight() const noexcept { return m_Height; }
        inline std::string ToString() const noexcept
        {
            return fmt::format("WindowResizeEvent: Width({}), Height({})", m_Width, m_Height);
        }

        EVENT_CLASS_TYPE(WindowResize)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    };

    class CODEX_API WindowCloseEvent : public Event
    {
    public:
        WindowCloseEvent() = default;

        EVENT_CLASS_TYPE(WindowClose)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    };
} // namespace codex::events

#endif // CODEX_EVENTS_APPLICATION_EVENT_H
