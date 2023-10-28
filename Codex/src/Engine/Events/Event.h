#ifndef CODEX_EVENTS_EVENT_H
#define CODEX_EVENTS_EVENT_H

#include <sdafx.h>

#define EVENT_CLASS_TYPE(type)                                                                                         \
    static EventType GetStaticType()                                                                                   \
    {                                                                                                                  \
        return EventType::type;                                                                                        \
    }                                                                                                                  \
    EventType GetType() const noexcept override                                                                        \
    {                                                                                                                  \
        return GetStaticType();                                                                                        \
    }                                                                                                                  \
    const char* GetName() const noexcept override                                                                      \
    {                                                                                                                  \
        return #type;                                                                                                  \
    }

#define EVENT_CLASS_CATEGORY(category)                                                                                 \
    u32 GetCategory() const noexcept override                                                                          \
    {                                                                                                                  \
        return category;                                                                                               \
    }

namespace codex {
    enum class EventType : u8
    {
        None = 0,

        // Window events.
        WindowClose,
        WindowResize,
        WindowFocus,
        WindowLostFocus,
        WindowMove,

        // Application events.
        AppTick,
        AppUpdate,
        AppRender,

        // Key evennts.
        KeyDown,
        KeyUp,

        // Mouse events.
        MouseDown,
        MouseUp,
        MouseMove,
        MouseScroll
    };

    enum EventCategory : u32
    {
        None                     = 0,
        EventCategoryApplication = BitFlag(0),
        EventCategoryInput       = BitFlag(1),
        EventCategoryKeyboard    = BitFlag(2),
        EventCategoryMouse       = BitFlag(3),
        EventCategoryMouseButton = BitFlag(4)
    };

    class Event
    {
    public:
        bool handled = false;

    public:
        virtual ~Event() = default;

    public:
        virtual EventType   GetType() const noexcept     = 0;
        virtual const char* GetName() const noexcept     = 0;
        virtual u32         GetCategory() const noexcept = 0;
        virtual std::string ToString() const noexcept { return GetName(); }

    public:
        inline bool IsInCategory(const EventCategory category) { return GetCategory() & category; }
    };

    class EventDispatcher
    {
    private:
        Event& m_Event;

    public:
        EventDispatcher(Event& event) : m_Event(event) {}

    public:
        template <typename T, typename F>
        bool Dispatch(const F& delegate)
        {
            if (m_Event.GetType() == T::GetStaticType())
            {
                m_Event.handled |= delegate((T&)m_Event);
                return true;
            }
            return false;
        }
    };
} // namespace codex

namespace fmt {
    template <>
    struct formatter<codex::Event>
    {
        template <typename ParseContext>
        constexpr auto parse(ParseContext& ctx)
        {
            return ctx.begin();
        }

        template <typename FormatContext>
        auto format(const codex::Event& event, FormatContext& ctx)
        {
            static const char* type_str_arr[] = { "None",

                                                  // Window events.
                                                  "WindowClose", "WindowResize", "WindowFocus", "WindowLostFocus",
                                                  "WindowMove",

                                                  // Application events.
                                                  "AppTick", "AppUpdate", "AppRender",

                                                  // Key evennts.
                                                  "KeyDown", "KeyUp",

                                                  // Mouse events.
                                                  "MouseDown", "MouseUp", "MouseMove", "MouseScroll" };
            return fmt::format_to(ctx.out(), "(Name: {}, Type: {})", event.GetName(),
                                  type_str_arr[(codex::usize)event.GetType()]);
        }
    };
} // namespace fmt

#endif // CODEX_EVENTS_EVENT_H
