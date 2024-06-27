#include "Input.h"
#include "../Events/KeyEvent.h"
#include "../Events/MouseEvent.h"

namespace codex {
    using namespace codex::events;

    const char* KeyToString(const Key key) noexcept
    {
        static const std::unordered_map<Key, const char*> key_str_map = {
            { Key::Unknown, "Unknown" },
            { Key::Return, "Return" },
            { Key::CapsLock, "CapsLock" },
            { Key::Escape, "Escape" },
            { Key::Backspace, "Backspace" },
            { Key::Tab, "Tab" },
            { Key::Space, "Space" },
            { Key::Exclaim, "Exclaim" },
            { Key::DoubleQuote, "DoubleQuote" },
            { Key::Pound, "Pound" },
            { Key::Percent, "Percent" },
            { Key::Dollar, "Dollar" },
            { Key::Ampersand, "Ampersand" },
            { Key::SingleQuote, "SingleQuote" },
            { Key::LeftParen, "LeftParen" },
            { Key::RightParen, "RightParen" },
            { Key::Asterisk, "Asterisk" },
            { Key::Plus, "Plus" },
            { Key::Comma, "Comma" },
            { Key::Minus, "Minus" },
            { Key::Period, "Period" },
            { Key::ForwardSlash, "ForwardSlash" },
            { Key::Num0, "Num0" },
            { Key::Num1, "Num1" },
            { Key::Num2, "Num2" },
            { Key::Num3, "Num3" },
            { Key::Num4, "Num4" },
            { Key::Num5, "Num5" },
            { Key::Num6, "Num6" },
            { Key::Num7, "Num7" },
            { Key::Num8, "Num8" },
            { Key::Num9, "Num9" },
            { Key::Colon, "Colon" },
            { Key::SemiColon, "SemiColon" },
            { Key::Less, "Less" },
            { Key::Equals, "Equals" },
            { Key::Greater, "Greater" },
            { Key::QuestionMark, "QuestionMark" },
            { Key::At, "At" },
            { Key::LeftBracket, "LeftBracket" },
            { Key::BackSlash, "BackSlash" },
            { Key::RightBracket, "RightBracket" },
            { Key::Caret, "Caret" },
            { Key::Underscore, "Underscore" },
            { Key::BackQuote, "BackQuote" },
            { Key::A, "A" },
            { Key::B, "B" },
            { Key::C, "C" },
            { Key::D, "D" },
            { Key::E, "E" },
            { Key::F, "F" },
            { Key::G, "G" },
            { Key::H, "H" },
            { Key::I, "I" },
            { Key::J, "J" },
            { Key::K, "K" },
            { Key::L, "L" },
            { Key::M, "M" },
            { Key::N, "N" },
            { Key::O, "O" },
            { Key::P, "P" },
            { Key::Q, "Q" },
            { Key::R, "R" },
            { Key::S, "S" },
            { Key::T, "T" },
            { Key::U, "U" },
            { Key::V, "V" },
            { Key::W, "W" },
            { Key::X, "X" },
            { Key::Y, "Y" },
            { Key::Z, "Z" },
            { Key::F1, "F1" },
            { Key::F2, "F2" },
            { Key::F3, "F3" },
            { Key::F4, "F4" },
            { Key::F5, "F5" },
            { Key::F6, "F6" },
            { Key::F7, "F7" },
            { Key::F8, "F8" },
            { Key::F9, "F9" },
            { Key::F10, "F10" },
            { Key::F11, "F11" },
            { Key::F12, "F12" },
            { Key::PrintScreen, "PrintScreen" },
            { Key::ScrollLock, "ScrollLock" },
            { Key::Pause, "Pause" },
            { Key::Insert, "Insert" },
            { Key::Home, "Home" },
            { Key::PageUp, "PageUp" },
            { Key::Delete, "Delete" },
            { Key::End, "End" },
            { Key::PageDown, "PageDown" },
            { Key::Right, "Right" },
            { Key::Left, "Left" },
            { Key::Down, "Down" },
            { Key::Up, "Up" },
            { Key::NumLockClear, "NumLockClear" },
            { Key::KeyPadDivide, "KeyPadDivide" },
            { Key::KeyPadMultiply, "KeyPadMultiply" },
            { Key::KeyPadMinus, "KeyPadMinus" },
            { Key::KeyPadPlus, "KeyPadPlus" },
            { Key::KeyPadEnter, "KeyPadEnter" },
            { Key::KeyPad1, "KeyPad1" },
            { Key::KeyPad2, "KeyPad2" },
            { Key::KeyPad3, "KeyPad3" },
            { Key::KeyPad4, "KeyPad4" },
            { Key::KeyPad5, "KeyPad5" },
            { Key::KeyPad6, "KeyPad6" },
            { Key::KeyPad7, "KeyPad7" },
            { Key::KeyPad8, "KeyPad8" },
            { Key::KeyPad9, "KeyPad9" },
            { Key::KeyPad0, "KeyPad0" },
            { Key::KeyPadPeriod, "KeyPadPeriod" },
            { Key::Application, "Application" },
            { Key::Power, "Power" },
            { Key::KeyPadEquals, "KeyPadEquals" },
            { Key::F13, "F13" },
            { Key::F14, "F14" },
            { Key::F15, "F15" },
            { Key::F16, "F16" },
            { Key::F17, "F17" },
            { Key::F18, "F18" },
            { Key::F19, "F19" },
            { Key::F20, "F20" },
            { Key::F21, "F21" },
            { Key::F22, "F22" },
            { Key::F23, "F23" },
            { Key::F24, "F24" },
            { Key::Execute, "Execute" },
            { Key::Help, "Help" },
            { Key::Menu, "Menu" },
            { Key::Select, "Select" },
            { Key::Stop, "Stop" },
            { Key::Again, "Again" },
            { Key::Undo, "Undo" },
            { Key::Cut, "Cut" },
            { Key::Copy, "Copy" },
            { Key::Paste, "Paste" },
            { Key::Find, "Find" },
            { Key::Mute, "Mute" },
            { Key::VolumeUp, "VolumeUp" },
            { Key::VolumeDown, "VolumeDown" },
            { Key::KeyPadComma, "KeyPadComma" },
            { Key::KeyPadEqualsAs400, "KeyPadEqualsAs400" },
            { Key::Alterase, "Alterase" },
            { Key::SysReq, "SysReq" },
            { Key::Cancel, "Cancel" },
            { Key::Clear, "Clear" },
            { Key::Prior, "Prior" },
            { Key::Return2, "Return2" },
            { Key::Separator, "Separator" },
            { Key::Out, "Out" },
            { Key::Oper, "Oper" },
            { Key::ClearAgain, "ClearAgain" },
            { Key::CrSel, "CrSel" },
            { Key::ExSel, "ExSel" },
            { Key::KeyPad00, "KeyPad00" },
            { Key::KeyPad000, "KeyPad000" },
            { Key::ThousandsSeparator, "ThousandsSeparator" },
            { Key::DecimalSeparator, "DecimalSeparator" },
            { Key::CurrencyUnit, "CurrencyUnit" },
            { Key::CurrencySubUnit, "CurrencySubUnit" },
            { Key::KeyPadLeftParen, "KeyPadLeftParen" },
            { Key::KeyPadRightParen, "KeyPadRightParen" },
            { Key::KeyPadLeftBrace, "KeyPadLeftBrace" },
            { Key::KeyPadRightBrace, "KeyPadRightBrace" },
            { Key::KeyPadTab, "KeyPadTab" },
            { Key::KeyPadBackspace, "KeyPadBackspace" },
            { Key::KeyPadA, "KeyPadA" },
            { Key::KeyPadB, "KeyPadB" },
            { Key::KeyPadC, "KeyPadC" },
            { Key::KeyPadD, "KeyPadD" },
            { Key::KeyPadE, "KeyPadE" },
            { Key::KeyPadF, "KeyPadF" },
            { Key::KeyPadXOR, "KeyPadXOR" },
            { Key::KeyPadPower, "KeyPadPower" },
            { Key::KeyPadPercent, "KeyPadPercent" },
            { Key::KeyPadLess, "KeyPadLess" },
            { Key::KeyPadGreater, "KeyPadGreater" },
            { Key::KeyPadAmpersand, "KeyPadAmpersand" },
            { Key::KeyPadDblAmpersand, "KeyPadDblAmpersand" },
            { Key::KeyPadVerticalBar, "KeyPadVerticalBar" },
            { Key::KeyPadDblVerticalBar, "KeyPadDblVerticalBar" },
            { Key::KeyPadColon, "KeyPadColon" },
            { Key::KeyPadHash, "KeyPadHash" },
            { Key::KeyPadSpace, "KeyPadSpace" },
            { Key::KeyPadAt, "KeyPadAt" },
            { Key::KeyPadExclam, "KeyPadExclam" },
            { Key::KeyPadMemStore, "KeyPadMemStore" },
            { Key::KeyPadMemRecall, "KeyPadMemRecall" },
            { Key::KeyPadMemClear, "KeyPadMemClear" },
            { Key::KeyPadMemAdd, "KeyPadMemAdd" },
            { Key::KeyPadMemSubtract, "KeyPadMemSubtract" },
            { Key::KeyPadMemMultiply, "KeyPadMemMultiply" },
            { Key::KeyPadMemDivide, "KeyPadMemDivide" },
            { Key::KeyPadPlusMinus, "KeyPadPlusMinus" },
            { Key::KeyPadClear, "KeyPadClear" },
            { Key::KeyPadClearEntry, "KeyPadClearEntry" },
            { Key::KeyPadBinary, "KeyPadBinary" },
            { Key::KeyPadOctal, "KeyPadOctal" },
            { Key::KeyPadDecimal, "KeyPadDecimal" },
            { Key::KeyPadHexadecimal, "KeyPadHexadecimal" },
            { Key::LeftCtrl, "LeftCtrl" },
            { Key::LeftShift, "LeftShift" },
            { Key::LeftAlt, "LeftAlt" },
            { Key::LeftGUI, "LeftGUI" },
            { Key::RightCtrl, "RightCtrl" },
            { Key::RightShift, "RightShift" },
            { Key::RightAlt, "RightAlt" },
            { Key::RightGUI, "RightGUI" },
            { Key::Mode, "Mode" },
            { Key::AudioNext, "AudioNext" },
            { Key::AudioPrev, "AudioPrev" },
            { Key::AudioStop, "AudioStop" },
            { Key::AudioPlay, "AudioPlay" },
            { Key::AudioMute, "AudioMute" },
            { Key::MediaSelect, "MediaSelect" },
            { Key::WWW, "WWW" },
            { Key::Mail, "Mail" },
            { Key::Calculator, "Calculator" },
            { Key::Computer, "Computer" },
            { Key::AppControlSearch, "AppControlSearch" },
            { Key::AppControlHome, "AppControlHome" },
            { Key::AppControlBack, "AppControlBack" },
            { Key::AppControlForward, "AppControlForward" },
            { Key::AppControlStop, "AppControlStop" },
            { Key::AppControlRefresh, "AppControlRefresh" },
            { Key::AppControlBookmarks, "AppControlBookmarks" },
            { Key::BrightnessDown, "BrightnessDown" },
            { Key::BrightnessUp, "BrightnessUp" },
            { Key::DisplaySwitch, "DisplaySwitch" },
            { Key::KbdIllumToggle, "KbdIllumToggle" },
            { Key::KbdIllumDown, "KbdIllumDown" },
            { Key::KbdIllumUp, "KbdIllumUp" },
            { Key::Eject, "Eject" },
            { Key::Sleep, "Sleep" },
            { Key::App1, "App1" },
            { Key::App2, "App2" },
            { Key::AudioRewind, "AudioRewind" },
            { Key::AudioFastForward, "AudioFastForward" },
            { Key::SoftLeft, "SoftLeft" },
            { Key::SoftRight, "SoftRight" },
            { Key::Call, "Call" },
            { Key::EndCall, "EndCall" },
        };
        // Althought std::map::at(const T) does throw an exception.
        return key_str_map.at(key);
    }

    const char* MouseToString(const Mouse button) noexcept
    {
        static const char* mouse_str_arr[] = { "LeftMouse", "MiddleMouse", "RightMouse", "X1Mouse", "X2Mouse" };
        return mouse_str_arr[(usize)button - 1];
    }

    Input*                        Input::m_Instance = nullptr;
    std::unordered_map<Key, bool> Input::m_KeysDown;
    std::bitset<3>                Input::m_ButtonsDown;

    Input* Input::Get()
    {
        if (!m_Instance)
        {
            m_Instance = new Input();
            fmt::println("Input subsystem initialized.");
        }
        return m_Instance;
    }

    void Input::Destroy()
    {
        if (m_Instance)
        {
            delete m_Instance;
            m_Instance = nullptr;
            fmt::println("Input subsystem disposed.");
        }
    }

    bool Input::OnKeyDown_Event(const KeyDownEvent event)
    {
        m_Instance->m_KeysDown[event.GetKey()] = true;
        return false;
    }

    bool Input::OnKeyUp_Event(const KeyUpEvent event)
    {
        m_Instance->m_KeysDown[event.GetKey()] = false;
        return false;
    }

    bool Input::OnMouseMove_Event(const MouseMoveEvent event)
    {
        m_MouseLastPosX = m_MousePosX;
        m_MouseLastPosY = m_MousePosY;
        m_MousePosX     = event.GetX();
        m_MousePosY     = event.GetY();
        m_MouseDragging = m_ButtonsDown.any();
        return false;
    }

    bool Input::OnMouseUp_Event(const MouseUpEvent event)
    {
        m_ButtonsDown[(usize)event.GetMouseButton() - 1] = false;
        m_Instance->m_MouseDragging                      = false;
        return false;
    }

    bool Input::OnMouseDown_Event(const MouseDownEvent event)
    {
        m_ButtonsDown[(usize)event.GetMouseButton() - 1] = true;
        return false;
    }

    bool Input::OnMouseScroll_Event(const MouseScrollEvent event)
    {
        m_MouseScrollX = event.GetOffsetX();
        m_MouseScrollY = event.GetOffsetY();
        return false;
    }

    bool Input::IsKeyDown(const Key key)
    {
        return m_KeysDown[key];
    }

    bool Input::IsMouseDown(const Mouse button)
    {
        return m_ButtonsDown[(usize)button - 1];
    }
} // namespace codex