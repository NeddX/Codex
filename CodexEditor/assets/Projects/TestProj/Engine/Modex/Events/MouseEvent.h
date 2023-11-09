#ifndef CODEX_EVENTS_MOUSE_EVENT_H
#define CODEX_EVENTS_MOUSE_EVENT_H

#include <sdafx.h>

#include "../Core/Input.h"
#include "Event.h"

namespace codex {
    class MouseMoveEvent : public Event
    {
    private:
        i32 m_MousePosX;
        i32 m_MousePosY;

    public:
        MouseMoveEvent(const i32 x, const i32 y) : m_MousePosX(x), m_MousePosY(y) {}

    public:
        inline i32 GetX() const noexcept { return m_MousePosX; }
        inline i32 GetY() const noexcept { return m_MousePosY; }

    public:
        std::string ToString() const noexcept override
        {
            return fmt::format("MouseMoveEvent: X({}), Y({})", m_MousePosX, m_MousePosY);
        }

        EVENT_CLASS_TYPE(MouseMove)
        EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
    };

    class MouseButtonEvent : public Event
    {
    protected:
        Mouse m_Button;
        i32   m_MouseX;
        i32   m_MouseY;

    protected:
        MouseButtonEvent(const Mouse button, const i32 mouseX, const i32 mouseY)
            : m_Button(button), m_MouseX(mouseX), m_MouseY(mouseY)
        {
        }

    public:
        inline Mouse GetMouseButton() const noexcept { return m_Button; }
        inline i32   GetMouseX() const noexcept { return m_MouseX; }
        inline i32   GetMouseY() const noexcept { return m_MouseY; }

        EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput | EventCategoryMouseButton)
    };

    class MouseScrollEvent : public MouseButtonEvent
    {
    private:
        i32 m_OffsetX;
        i32 m_OffsetY;

    public:
        MouseScrollEvent(const Mouse button, const i32 mouseX, const i32 mouseY, const i32 offsetX, const i32 offsetY)
            : MouseButtonEvent(button, mouseX, mouseY), m_OffsetX(offsetX), m_OffsetY(offsetY)
        {
        }

    public:
        inline i32 GetOffsetX() const noexcept { return m_OffsetX; }
        inline i32 GetOffsetY() const noexcept { return m_OffsetY; }

    public:
        std::string ToString() const noexcept override
        {
            return fmt::format("MouseScrollEvent: OffsetX({}), OffsetY({})", m_OffsetX, m_OffsetY);
        }

        EVENT_CLASS_TYPE(MouseScroll)
        EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
    };

    class MouseDownEvent : public MouseButtonEvent
    {
    public:
        MouseDownEvent(const Mouse button, const i32 mouseX, const i32 mouseY)
            : MouseButtonEvent(button, mouseX, mouseY)
        {
        }

    public:
        std::string ToString() const noexcept override
        {
            return fmt::format("MouseDownEvent: Button({})", MouseToString(m_Button));
        }

        EVENT_CLASS_TYPE(MouseDown)
    };

    class MouseUpEvent : public MouseButtonEvent
    {
    public:
        MouseUpEvent(const Mouse button, const i32 mouseX, const i32 mouseY) : MouseButtonEvent(button, mouseX, mouseY)
        {
        }

    public:
        std::string ToString() const noexcept override
        {
            return fmt::format("MouseUpEvent: Button({})", MouseToString(m_Button));
        }

        EVENT_CLASS_TYPE(MouseUp)
    };
} // namespace codex

#endif // CODEX_EVENTS_MOUSE_EVENT_H
