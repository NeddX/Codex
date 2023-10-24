#ifndef CODEX_CORE_MOUSE_HANDLER_H
#define CODEX_CORE_MOUSE_HANDLER_H

#include <sdafx.h>

#include "Geomtryd.h"

namespace codex {
    // Forward decelerations
    class Window;

    enum class MouseButton : u8
    {
        LeftMouse   = SDL_BUTTON_LEFT,
        MiddleMouse = SDL_BUTTON_MIDDLE,
        RightMouse  = SDL_BUTTON_RIGHT,
        X1Mouse     = SDL_BUTTON_X1,
        X2Mouse     = SDL_BUTTON_X2
    };

    class MouseHandler
    {
        friend class Window;

    private:
        static MouseHandler* m_Instance;

    private:
        i32            m_ScrollX, m_ScrollY;
        i32            m_PosX, m_PosY, m_LastPosX, m_LastPosY;
        std::bitset<3> m_ButtonsPressed;
        bool           m_Dragging, m_Disposed;

    private:
        MouseHandler();
        ~MouseHandler();

    protected:
        struct MouseEvent
        {
            i32 x, y, scrollX, scrollY;
            u8  action, button, clicks;
            u32 scrollDir;
        };

    public:
        static inline i32      GetMouseX() noexcept { return m_Instance->m_PosX; }
        static inline i32      GetMouseY() noexcept { return m_Instance->m_PosY; }
        static inline Vector2f GetMousePos() noexcept { return Vector2f(GetMouseX(), GetMouseY()); }
        static inline i32      GetMouseDeltaX() noexcept { return m_Instance->m_LastPosX - m_Instance->m_PosX; }
        static inline i32      GetMouseDeltaY() noexcept { return m_Instance->m_LastPosY - m_Instance->m_PosY; }
        static inline i32      GetScrollX() noexcept { return m_Instance->m_ScrollX; }
        static inline i32      GetScrollY() noexcept { return m_Instance->m_ScrollY; }
        static inline bool     IsDragging() noexcept { return m_Instance->m_Dragging; }
        static inline bool     IsMouseDown(const MouseButton mouseButton) noexcept
        {
            return m_Instance->m_ButtonsPressed[(usize)mouseButton];
        }

    protected:
        static void Init() noexcept;
        static void Destroy() noexcept;

    protected:
        void EndFrame() noexcept;

    protected:
        static void OnMouseMove_Event(const MouseEvent event) noexcept;
        static void OnMouseButton_Event(const MouseEvent event) noexcept;
        static void OnMouseScroll_Event(const MouseEvent event) noexcept;
    };
} // namespace codex

#endif // CODEX_CORE_MOUSE_HANDLER_H
