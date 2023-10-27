#ifndef CODEX_CORE_WINDOW_H
#define CODEX_CORE_WINDOW_H

#include <sdafx.h>

#include "../Renderer/Renderer.h"
#include "../Scene/ECS.h"
#include "../Scene/Scene.h"
#include "CommonDef.h"
#include "Exception.h"

namespace codex {
    // Forward declerations
    class Application;

    class SDLException : public CodexException
    {
        using CodexException::CodexException;

    public:
        inline const char* default_message() const noexcept override { return "SDL failed to initialize."; }
    };

    class GLADException : public CodexException
    {
        using CodexException::CodexException;

    public:
        inline const char* default_message() const noexcept override { return "GLAD failed to initialize."; }
    };

    struct WindowProperties
    {
        const char* title      = "Codex - Window";
        i32         width      = 1280;
        i32         height     = 720;
        i32         posX       = SDL_WINDOWPOS_CENTERED;
        i32         posY       = SDL_WINDOWPOS_CENTERED;
        u32         frameCap   = 300;
        u32         flags      = SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;
        bool        vsync      = true;
        bool        borderless = false;
    };

    class Window
    {
        using Box = std::unique_ptr<Window, std::function<void(Window*)>>;

        friend class Application;

    private:
        std::string                           m_Title;
        i32                                   m_Width, m_Height;
        i32                                   m_PosX, m_PosY;
        u32                                   m_Flags;
        u32                                   m_Fps, m_FrameCount, m_FrameCap;
        std::chrono::system_clock::time_point m_Tp1, m_Tp2;
        const void*                           m_NativeWindow;
        std::unique_ptr<Scene>                m_CurrentScene;
        std::unique_ptr<Renderer>             m_Renderer;
        SDL_Window*                           m_SdlWindow;
        SDL_GLContext                         m_GlContext;
        SDL_Event                             m_SdlEvent;

    private:
        Window();
        Window(const Window& other)                = delete;
        Window& operator=(const Window& other)     = delete;
        Window(Window&& other) noexcept            = delete;
        Window& operator=(Window&& other) noexcept = delete;
        ~Window();

    public:
        inline i32            GetWidth() const noexcept { return m_Width; }
        inline i32            GetHeight() const noexcept { return m_Height; }
        inline Scene*         GetCurrentScene() const noexcept { return m_CurrentScene.get(); }
        inline void           SetTitle(const char* newTitle) noexcept { SDL_SetWindowTitle(m_SdlWindow, newTitle); }
        inline SDL_Window*    GetNativeWindow() noexcept { return m_SdlWindow; }
        inline SDL_GLContext* GetGlContext() noexcept { return &m_GlContext; }

    public:
        void Init(const WindowProperties windowInfo = WindowProperties(), const void* nativeWindow = nullptr);
        void Update(const f32 delta_time);
        void ProcessEvents();
        void ChangeScene(const i32 sceneId);
        void SDLCheckError(const i32 line = -1);
        void SDLThrowError(const i32 line, const std::string_view errorMessage);
        void OnWindowResize_Event(const i32 newWidth, const i32 newHeight);
    };
} // namespace codex

#endif // CODEX_CORE_WINDOW_H
