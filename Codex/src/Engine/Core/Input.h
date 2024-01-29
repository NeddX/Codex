#ifndef CODEX_CORE_INPUT_H
#define CODEX_CORE_INPUT_H

#include <sdafx.h>

#include "Application.h"
#include "Geomtryd.h"

namespace codex {
    // Forward decelrations.
    namespace events {
        class KeyDownEvent;
        class KeyUpEvent;
        class MouseDownEvent;
        class MouseUpEvent;
        class MouseMoveEvent;
        class MouseScrollEvent;
    } // namespace events

    enum class Key
    {
        Unknown = SDLK_UNKNOWN,

        Return       = SDLK_RETURN,
        CapsLock     = SDLK_CAPSLOCK,
        Escape       = SDLK_ESCAPE,
        Backspace    = SDLK_BACKSPACE,
        Tab          = SDLK_TAB,
        Space        = SDLK_SPACE,
        Exclaim      = SDLK_EXCLAIM,
        DoubleQuote  = SDLK_QUOTEDBL,
        Pound        = SDLK_HASH,
        Percent      = SDLK_PERCENT,
        Dollar       = SDLK_DOLLAR,
        Ampersand    = SDLK_AMPERSAND,
        SingleQuote  = SDLK_QUOTE,
        LeftParen    = SDLK_LEFTPAREN,
        RightParen   = SDLK_RIGHTPAREN,
        Asterisk     = SDLK_ASTERISK,
        Plus         = SDLK_PLUS,
        Comma        = SDLK_COMMA,
        Minus        = SDLK_MINUS,
        Period       = SDLK_PERIOD,
        ForwardSlash = SDLK_SLASH,
        Num0         = SDLK_0,
        Num1         = SDLK_1,
        Num2         = SDLK_2,
        Num3         = SDLK_3,
        Num4         = SDLK_4,
        Num5         = SDLK_5,
        Num6         = SDLK_6,
        Num7         = SDLK_7,
        Num8         = SDLK_8,
        Num9         = SDLK_9,
        Colon        = SDLK_COLON,
        SemiColon    = SDLK_SEMICOLON,
        Less         = SDLK_LESS,
        Equals       = SDLK_EQUALS,
        Greater      = SDLK_GREATER,
        QuestionMark = SDLK_QUESTION,
        At           = SDLK_AT,

        /*
           Skip uppercase letters
         */

        LeftBracket  = SDLK_LEFTBRACKET,
        BackSlash    = SDLK_BACKSLASH,
        RightBracket = SDLK_RIGHTBRACKET,
        Caret        = SDLK_CARET,
        Underscore   = SDLK_UNDERSCORE,
        BackQuote    = SDLK_BACKQUOTE,
        A            = SDLK_a,
        B            = SDLK_b,
        C            = SDLK_c,
        D            = SDLK_d,
        E            = SDLK_e,
        F            = SDLK_f,
        G            = SDLK_g,
        H            = SDLK_h,
        I            = SDLK_i,
        J            = SDLK_j,
        K            = SDLK_k,
        L            = SDLK_l,
        M            = SDLK_m,
        N            = SDLK_n,
        O            = SDLK_o,
        P            = SDLK_p,
        Q            = SDLK_q,
        R            = SDLK_r,
        S            = SDLK_s,
        T            = SDLK_t,
        U            = SDLK_u,
        V            = SDLK_v,
        W            = SDLK_w,
        X            = SDLK_x,
        Y            = SDLK_y,
        Z            = SDLK_z,

        F1  = SDLK_F1,
        F2  = SDLK_F2,
        F3  = SDLK_F3,
        F4  = SDLK_F4,
        F5  = SDLK_F5,
        F6  = SDLK_F6,
        F7  = SDLK_F7,
        F8  = SDLK_F8,
        F9  = SDLK_F9,
        F10 = SDLK_F10,
        F11 = SDLK_F11,
        F12 = SDLK_F12,

        PrintScreen = SDLK_PRINTSCREEN,
        ScrollLock  = SDLK_SCROLLLOCK,
        Pause       = SDLK_PAUSE,
        Insert      = SDLK_INSERT,
        Home        = SDLK_HOME,
        PageUp      = SDLK_PAGEUP,
        Delete      = SDLK_DELETE,
        End         = SDLK_END,
        PageDown    = SDLK_PAGEDOWN,
        Right       = SDLK_RIGHT,
        Left        = SDLK_LEFT,
        Down        = SDLK_DOWN,
        Up          = SDLK_UP,

        NumLockClear   = SDLK_NUMLOCKCLEAR,
        KeyPadDivide   = SDLK_KP_DIVIDE,
        KeyPadMultiply = SDLK_KP_MULTIPLY,
        KeyPadMinus    = SDLK_KP_MINUS,
        KeyPadPlus     = SDLK_KP_PLUS,
        KeyPadEnter    = SDLK_KP_ENTER,
        KeyPad1        = SDLK_KP_1,
        KeyPad2        = SDLK_KP_2,
        KeyPad3        = SDLK_KP_3,
        KeyPad4        = SDLK_KP_4,
        KeyPad5        = SDLK_KP_5,
        KeyPad6        = SDLK_KP_6,
        KeyPad7        = SDLK_KP_7,
        KeyPad8        = SDLK_KP_8,
        KeyPad9        = SDLK_KP_9,
        KeyPad0        = SDLK_KP_0,
        KeyPadPeriod   = SDLK_KP_PERIOD,

        Application       = SDLK_APPLICATION,
        Power             = SDLK_POWER,
        KeyPadEquals      = SDLK_KP_EQUALS,
        F13               = SDLK_F13,
        F14               = SDLK_F14,
        F15               = SDLK_F15,
        F16               = SDLK_F16,
        F17               = SDLK_F17,
        F18               = SDLK_F18,
        F19               = SDLK_F19,
        F20               = SDLK_F20,
        F21               = SDLK_F21,
        F22               = SDLK_F22,
        F23               = SDLK_F23,
        F24               = SDLK_F24,
        Execute           = SDLK_EXECUTE,
        Help              = SDLK_HELP,
        Menu              = SDLK_MENU,
        Select            = SDLK_SELECT,
        Stop              = SDLK_STOP,
        Again             = SDLK_AGAIN,
        Undo              = SDLK_UNDO,
        Cut               = SDLK_CUT,
        Copy              = SDLK_COPY,
        Paste             = SDLK_PASTE,
        Find              = SDLK_FIND,
        Mute              = SDLK_MUTE,
        VolumeUp          = SDLK_VOLUMEUP,
        VolumeDown        = SDLK_VOLUMEDOWN,
        KeyPadComma       = SDLK_KP_COMMA,
        KeyPadEqualsAs400 = SDLK_KP_EQUALSAS400,

        Alterase   = SDLK_ALTERASE,
        SysReq     = SDLK_SYSREQ,
        Cancel     = SDLK_CANCEL,
        Clear      = SDLK_CLEAR,
        Prior      = SDLK_PRIOR,
        Return2    = SDLK_RETURN2,
        Separator  = SDLK_SEPARATOR,
        Out        = SDLK_OUT,
        Oper       = SDLK_OPER,
        ClearAgain = SDLK_CLEARAGAIN,
        CrSel      = SDLK_CRSEL,
        ExSel      = SDLK_EXSEL,

        KeyPad00             = SDLK_KP_00,
        KeyPad000            = SDLK_KP_000,
        ThousandsSeparator   = SDLK_THOUSANDSSEPARATOR,
        DecimalSeparator     = SDLK_DECIMALSEPARATOR,
        CurrencyUnit         = SDLK_CURRENCYUNIT,
        CurrencySubUnit      = SDLK_CURRENCYSUBUNIT,
        KeyPadLeftParen      = SDLK_KP_LEFTPAREN,
        KeyPadRightParen     = SDLK_KP_RIGHTPAREN,
        KeyPadLeftBrace      = SDLK_KP_LEFTBRACE,
        KeyPadRightBrace     = SDLK_KP_RIGHTBRACE,
        KeyPadTab            = SDLK_KP_TAB,
        KeyPadBackspace      = SDLK_KP_BACKSPACE,
        KeyPadA              = SDLK_KP_A,
        KeyPadB              = SDLK_KP_B,
        KeyPadC              = SDLK_KP_C,
        KeyPadD              = SDLK_KP_D,
        KeyPadE              = SDLK_KP_E,
        KeyPadF              = SDLK_KP_F,
        KeyPadXOR            = SDLK_KP_XOR,
        KeyPadPower          = SDLK_KP_POWER,
        KeyPadPercent        = SDLK_KP_PERCENT,
        KeyPadLess           = SDLK_KP_LESS,
        KeyPadGreater        = SDLK_KP_GREATER,
        KeyPadAmpersand      = SDLK_KP_AMPERSAND,
        KeyPadDblAmpersand   = SDLK_KP_DBLAMPERSAND,
        KeyPadVerticalBar    = SDLK_KP_VERTICALBAR,
        KeyPadDblVerticalBar = SDLK_KP_DBLVERTICALBAR,
        KeyPadColon          = SDLK_KP_COLON,
        KeyPadHash           = SDLK_KP_HASH,
        KeyPadSpace          = SDLK_KP_SPACE,
        KeyPadAt             = SDLK_KP_AT,
        KeyPadExclam         = SDLK_KP_EXCLAM,
        KeyPadMemStore       = SDLK_KP_MEMSTORE,
        KeyPadMemRecall      = SDLK_KP_MEMRECALL,
        KeyPadMemClear       = SDLK_KP_MEMCLEAR,
        KeyPadMemAdd         = SDLK_KP_MEMADD,
        KeyPadMemSubtract    = SDLK_KP_MEMSUBTRACT,
        KeyPadMemMultiply    = SDLK_KP_MEMMULTIPLY,
        KeyPadMemDivide      = SDLK_KP_MEMDIVIDE,
        KeyPadPlusMinus      = SDLK_KP_PLUSMINUS,
        KeyPadClear          = SDLK_KP_CLEAR,
        KeyPadClearEntry     = SDLK_KP_CLEARENTRY,
        KeyPadBinary         = SDLK_KP_BINARY,
        KeyPadOctal          = SDLK_KP_OCTAL,
        KeyPadDecimal        = SDLK_KP_DECIMAL,
        KeyPadHexadecimal    = SDLK_KP_HEXADECIMAL,

        LeftCtrl   = SDLK_LCTRL,
        LeftShift  = SDLK_LSHIFT,
        LeftAlt    = SDLK_LALT,
        LeftGUI    = SDLK_LGUI,
        RightCtrl  = SDLK_RCTRL,
        RightShift = SDLK_RSHIFT,
        RightAlt   = SDLK_RALT,
        RightGUI   = SDLK_RGUI,

        Mode                = SDLK_MODE,
        AudioNext           = SDLK_AUDIONEXT,
        AudioPrev           = SDLK_AUDIOPREV,
        AudioStop           = SDLK_AUDIOSTOP,
        AudioPlay           = SDLK_AUDIOPLAY,
        AudioMute           = SDLK_AUDIOMUTE,
        MediaSelect         = SDLK_MEDIASELECT,
        WWW                 = SDLK_WWW,
        Mail                = SDLK_MAIL,
        Calculator          = SDLK_CALCULATOR,
        Computer            = SDLK_COMPUTER,
        AppControlSearch    = SDLK_AC_SEARCH,
        AppControlHome      = SDLK_AC_HOME,
        AppControlBack      = SDLK_AC_BACK,
        AppControlForward   = SDLK_AC_FORWARD,
        AppControlStop      = SDLK_AC_STOP,
        AppControlRefresh   = SDLK_AC_REFRESH,
        AppControlBookmarks = SDLK_AC_BOOKMARKS,

        BrightnessDown   = SDLK_BRIGHTNESSDOWN,
        BrightnessUp     = SDLK_BRIGHTNESSUP,
        DisplaySwitch    = SDLK_DISPLAYSWITCH,
        KbdIllumToggle   = SDLK_KBDILLUMTOGGLE,
        KbdIllumDown     = SDLK_KBDILLUMDOWN,
        KbdIllumUp       = SDLK_KBDILLUMUP,
        Eject            = SDLK_EJECT,
        Sleep            = SDLK_SLEEP,
        App1             = SDLK_APP1,
        App2             = SDLK_APP2,
        AudioRewind      = SDLK_AUDIOREWIND,
        AudioFastForward = SDLK_AUDIOFASTFORWARD,
        SoftLeft         = SDLK_SOFTLEFT,
        SoftRight        = SDLK_SOFTRIGHT,
        Call             = SDLK_CALL,
        EndCall          = SDLK_ENDCALL,
    };

    const char* KeyToString(const Key key) noexcept;

    enum class Mouse : u8
    {
        LeftMouse   = SDL_BUTTON_LEFT,
        MiddleMouse = SDL_BUTTON_MIDDLE,
        RightMouse  = SDL_BUTTON_RIGHT,
        X1Mouse     = SDL_BUTTON_X1,
        X2Mouse     = SDL_BUTTON_X2
    };

    const char* MouseToString(const Mouse button) noexcept;

    class CODEX_API Input
    {
    private:
        static Input*                        m_Instance;
        static std::unordered_map<Key, bool> m_KeysDown;
        static std::bitset<3>                m_ButtonsDown;

    private:
        i32  m_MousePosX     = 0;
        i32  m_MousePosY     = 0;
        i32  m_MouseLastPosX = 0;
        i32  m_MouseLastPosY = 0;
        i32  m_MouseScrollX  = 0;
        i32  m_MouseScrollY  = 0;
        bool m_MouseDragging = false;

    public:
        static Input* Get();
        static void   Destroy();
        static bool   IsKeyDown(const Key key);
        static bool   IsMouseDown(const Mouse button);

    public:
        static inline i32      GetMouseX() noexcept { return m_Instance->m_MousePosX; }
        static inline i32      GetMouseY() noexcept { return m_Instance->m_MousePosY; }
        static inline Vector2f GetMouseDelta() noexcept
        {
            return Vector2f((f32)m_Instance->m_MouseLastPosX - (f32)m_Instance->m_MousePosX,
                            (f32)m_Instance->m_MouseLastPosY - (f32)m_Instance->m_MousePosY);
        }
        static inline f32 GetMouseDeltaX() noexcept
        {
            return (f32)m_Instance->m_MouseLastPosX - (f32)m_Instance->m_MousePosX;
        }
        static inline f32 GetMouseDeltaY() noexcept
        {
            return (f32)m_Instance->m_MouseLastPosY - (f32)m_Instance->m_MousePosY;
        }
        static inline Vector2 GetMousePos() noexcept { return Vector2(GetMouseX(), GetMouseY()); }
        static inline Vector2 GetScreenMousePos() noexcept
        {
            Vector2 vec;
            SDL_GetGlobalMouseState(&vec.x, &vec.y);
            return vec;
        }
        static inline i32  GetScrollX() noexcept { return m_Instance->m_MouseScrollX; }
        static inline i32  GetScrollY() noexcept { return m_Instance->m_MouseScrollY; }
        static inline bool IsMouseDragging() noexcept { return m_Instance->m_MouseDragging; }
        static inline void EndFrame() noexcept
        {
            m_Instance->m_MouseLastPosX = m_Instance->m_MousePosX;
            m_Instance->m_MouseLastPosY = m_Instance->m_MousePosY;
            m_Instance->m_MouseScrollX  = 0;
            m_Instance->m_MouseScrollY  = 0;
        }

    public:
        bool OnKeyDown_Event(const events::KeyDownEvent event);
        bool OnKeyUp_Event(const events::KeyUpEvent event);
        bool OnMouseDown_Event(const events::MouseDownEvent event);
        bool OnMouseUp_Event(const events::MouseUpEvent event);
        bool OnMouseMove_Event(const events::MouseMoveEvent event);
        bool OnMouseScroll_Event(const events::MouseScrollEvent event);
    };
} // namespace codex

namespace fmt {
    template <>
    struct formatter<codex::Key>
    {
        template <typename ParseContext>
        constexpr auto parse(ParseContext& ctx)
        {
            return ctx.begin();
        }

        template <typename FormatContext>
        auto format(const codex::Key& key, FormatContext& ctx)
        {
            return fmt::format_to(ctx.out(), "{}", codex::KeyToString(key));
        }
    };
} // namespace fmt

#endif // CODEX_CORE_INPUT_H
