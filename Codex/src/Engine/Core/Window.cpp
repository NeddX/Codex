#include "../Events/ApplicationEvent.h"
#include "../Events/KeyEvent.h"
#include "../Events/MouseEvent.h"
#include "../Renderer/DebugDraw.h"
#include "Application.h"
#include "Input.h"

#include <imgui.h>
#include <imgui_impl_sdl2.h>

namespace codex {
    using namespace codex::events;
    using namespace codex::imgui;
    using namespace codex::graphics;

    WindowFlags operator|(const WindowFlags& lhv, const WindowFlags& rhv) noexcept
    {
        return (WindowFlags)((u32)lhv | (u32)rhv);
    }

    Window::Window()
    {
    }

    Window::~Window()
    {
        SDL_GL_DeleteContext(m_GlContext);
        SDL_DestroyWindow(m_SdlWindow);
        SDL_Quit();
    }

    void Window::Init(const WindowProperties& windowInfo, const void* nativeWindow)
    {
        m_NativeWindow = nativeWindow;
        m_Title        = windowInfo.title;
        m_Width        = windowInfo.width;
        m_Height       = windowInfo.height;
        m_PosX         = windowInfo.posX;
        m_PosY         = windowInfo.posY;
        m_FrameCap     = windowInfo.frameCap;
        m_Flags        = windowInfo.flags;
        m_Fps          = 0;
        m_FrameCount   = 0;
        m_Tp1          = std::chrono::system_clock::now();
        m_Tp2          = m_Tp1;

        // Initialize SDL and OpenGL
        if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
        {
            cx_throwd(SDLException);
            // SDLThrowError(__LINE__, "ERROR: FAILED TO INITIALIZE SDL!");
            return;
        }
        SDLCheckError(__LINE__);

        // Tell SDL to use OpenGL 3.3.0 Core
#ifdef CX_PLATFORM_OSX
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Always required on Mac
#endif
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

        // Create SDL Window.
        if (m_NativeWindow)
        {
            // m_SdlWindow = SDL_CreateWindowFrom(m_NativeWindow, m_Flags | SDL_WINDOW_OPENGL);
            fmt::println("Creating an SDL Window from a native window. Native window address {}",
                         (void*)m_NativeWindow);
            cx_throw(SDLException, "Native windows are not supported.");
        }
        else
            m_SdlWindow = SDL_CreateWindow(m_Title.c_str(), m_PosX, m_PosY, m_Width, m_Height,
                                           (u32)m_Flags | SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI);

        if (!m_SdlWindow)
        {
            cx_throw(SDLException, "Failed to create an SDL window.");
            // SDLThrowError(__LINE__, "ERROR: FAILED TO CREATE SDL WINDOW!");
        }
        SDLCheckError(__LINE__);

        // Create OpenGL context from SDL Window
        m_GlContext = SDL_GL_CreateContext(m_SdlWindow); // fails here
        if (!m_GlContext)
        {
            cx_throw(SDLException, "Failed to create an OpenGL context from the SDL window.");
            // SDLThrowError(__LINE__, "ERROR: FAILED TO CREATE AN OPENGL CONTEXT FROM SDL WINDOW!");
        }
        SDLCheckError(__LINE__);

        // Enable VSync
        SDL_GL_SetSwapInterval((windowInfo.vsync) ? 1 : 0);

        // Initialize GLAD
        if (gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
        {
            gladLoadGL();
            gladLoadGLLoader(SDL_GL_GetProcAddress);
            fmt::println("GLad loaded.\nVendor:\t\t{}\nRenderer:\t{}\nVersion:\tP{}",
                         (const char*)glGetString(GL_VENDOR), (const char*)glGetString(GL_RENDERER),
                         (const char*)glGetString(GL_VERSION));
        }
        else
        {
            cx_throwd(GLADException);
        }

        // MISC
        // TODO: When in editor mode, the initial window size is 0 by 0 which causes
        // gl to crash when creating the framebuffer for texture picking.
        // m_TexPick = std::make_unique<TexturePicking>(GetWidth(), GetHeight());
        // m_BatcherShader = std::make_unique<Shader>("texture2d.glsl");

        fmt::println("Window subsystem initialized.");
    }

    void Window::SDLCheckError(const i32 line)
    {
#ifdef CX_CONFIG_DEBUG
        const char* error = SDL_GetError();
        if (*error != 0)
        {
            fmt::println("SDL ERROR @ LINE {}: {}", line, error);
            if (line != -1)
                fmt::println(" + line: {}", line);
            SDL_ClearError();
        }
#endif
    }

    void Window::SDLThrowError(const i32 line, const std::string_view errorMessage)
    {
        fmt::println("SDL ERROR @ LINE {}: {} -> {}", line, errorMessage, SDL_GetError());
        SDL_Quit();
        exit(-1);
    }

    void Window::ProcessEvents()
    {
        static Application& app   = Application::Get();
        static ImGuiLayer*  imgui = app.GetImGuiLayer();
        while (SDL_PollEvent(&m_SdlEvent))
        {
            if (imgui)
                ImGui_ImplSDL2_ProcessEvent(&m_SdlEvent);
            switch (m_SdlEvent.type)
            {
                case SDL_QUIT: {
                    Application::Get().Stop();
                    break;
                }
                case SDL_WINDOWEVENT: {
                    switch (m_SdlEvent.window.event)
                    {
                        case SDL_WINDOWEVENT_CLOSE: {
                            Application::Get().Stop();
                            break;
                        }
                        case SDL_WINDOWEVENT_RESIZED: {
                            i32               width  = m_SdlEvent.window.data1;
                            i32               height = m_SdlEvent.window.data2;
                            WindowResizeEvent e(width, height);
                            if (m_EventCallback)
                            {
                                m_EventCallback(e);
                            }
                            break;
                        }
                    }
                }
                case SDL_MOUSEMOTION: {
                    MouseMoveEvent e(m_SdlEvent.motion.x, m_SdlEvent.motion.y);
                    if (m_EventCallback)
                    {
                        m_EventCallback(e);
                    }
                    break;
                }
                case SDL_MOUSEBUTTONDOWN: {
                    MouseDownEvent e(Mouse(m_SdlEvent.button.button), m_SdlEvent.motion.x, m_SdlEvent.motion.y);
                    if (m_EventCallback)
                    {
                        m_EventCallback(e);
                    }
                    break;
                }
                case SDL_MOUSEBUTTONUP: {
                    MouseUpEvent e(Mouse(m_SdlEvent.button.button), m_SdlEvent.motion.x, m_SdlEvent.motion.y);
                    if (m_EventCallback)
                    {
                        m_EventCallback(e);
                    }
                    break;
                }
                case SDL_MOUSEWHEEL: {
                    MouseScrollEvent e(Mouse(m_SdlEvent.button.button - 1), m_SdlEvent.wheel.x, m_SdlEvent.wheel.y,
                                       m_SdlEvent.wheel.x, m_SdlEvent.wheel.y);
                    if (m_EventCallback)
                    {
                        m_EventCallback(e);
                    }
                    break;
                }
                case SDL_KEYDOWN: {
                    KeyDownEvent e(Key(m_SdlEvent.key.keysym.sym), m_SdlEvent.key.repeat);
                    if (m_EventCallback)
                    {
                        m_EventCallback(e);
                    }
                    break;
                }
                case SDL_KEYUP: {
                    KeyUpEvent e(Key(m_SdlEvent.key.keysym.sym));
                    if (m_EventCallback)
                    {
                        m_EventCallback(e);
                    }
                    break;
                }
            }
        }
    }

    void Window::Update(const f32 delta_time)
    {
        // static mgl::FrameBufferProperties props(GetWidth(), GetHeight(), { mgl::TextureFormat::RGBA8,
        // mgl::TextureFormat::RedInt32 }); static mgl::FrameBuffer* fb = new mgl::FrameBuffer(props);
        m_Renderer->SetClearColour(0.2f, 0.2f, 0.2f, 1.0f);
        m_Renderer->Clear();

#ifdef CODEX_CONF_DEBUG
        DebugDraw::Begin();
#endif

        // Poll events
        ProcessEvents();

        // Update scene
#ifdef CODEX_CONF_DEBUG
        DebugDraw::Render();
#endif

        /*
        if (MouseHandler::IsMouseDown(0))
        {
            // Gotta reverse the Y cause opengl is gay
            i32 x = MouseHandler::GetMouseX(), y = GetHeight() - MouseHandler::GetMouseY();
            i32 pixel = fb->ReadPixel(1, x, y);
            fmt::println("pixel: {}", pixel);
            return;
        }
        */

        if (m_FrameCap > 0)
            SDL_Delay((u32)(1.0f / (f32)m_FrameCap * 1000.0f));
        m_FrameCount++;
        SDL_GL_SwapWindow(m_SdlWindow);
    }

    void Window::SwapBuffers()
    {
        if (m_FrameCap > 0)
            SDL_Delay((u32)(1.0f / (f32)m_FrameCap * 1000.0f));
        m_FrameCount++;
        SDL_GL_SwapWindow(m_SdlWindow);
    }

    void Window::OnWindowResize_Event(const i32 newWidth, const i32 newHeight)
    {
        if (newWidth == 0 || newHeight == 0)
            return;
        glViewport(0, 0, newWidth, newHeight);
        // m_CurrentScene->OnWindowResize_Event(newWidth, newHeight);
    }
} // namespace codex
