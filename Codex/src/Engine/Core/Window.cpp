#include "Window.h"
#include "../Renderer/DebugDraw.h"
#include "../Renderer/TexturePicking.h"
#include "../Scene/EditorScene.h"
#include "../Scene/LevelScene.h"
#include "Application.h"
#include "KeyHandler.h"
#include "MouseHandler.h"

#include <imgui_impl_sdl2.h>

namespace codex {
    Window::Window()
    {
    }

    Window::~Window()
    {
        DebugDraw::Destroy();
        KeyHandler::Destroy();
        MouseHandler::Destroy();
        Resources::Destroy();
        SDL_GL_DeleteContext(m_GlContext);
        SDL_DestroyWindow(m_SdlWindow);
        SDL_Quit();
    }

    void Window::Init(const WindowProperties windowInfo, const void* nativeWindow)
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
            CX_THROW_DEF(SDLException);
            // SDLThrowError(__LINE__, "ERROR: FAILED TO INITIALIZE SDL!");
            return;
        }
        SDLCheckError(__LINE__);

        // Tell SDL to use OpenGL 3.3.0 Core
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

        // Create SDL Window.
        if (m_NativeWindow)
        {
            m_SdlWindow = SDL_CreateWindowFrom(m_NativeWindow, m_Flags | SDL_WINDOW_OPENGL);
            fmt::println("Creating an SDL Window from a native window. Native window address {}",
                         (void*)m_NativeWindow);
        }
        else
            m_SdlWindow =
                SDL_CreateWindow(m_Title.c_str(), m_PosX, m_PosY, m_Width, m_Height, m_Flags | SDL_WINDOW_OPENGL);

        if (!m_SdlWindow)
        {
            CX_THROW(SDLException, "Failed to create an SDL window.");
            // SDLThrowError(__LINE__, "ERROR: FAILED TO CREATE SDL WINDOW!");
        }
        SDLCheckError(__LINE__);

        // Create OpenGL context from SDL Window
        m_GlContext = SDL_GL_CreateContext(m_SdlWindow); // fails here
        if (!m_GlContext)
        {
            CX_THROW(SDLException, "Failed to create an OpenGL context from the SDL window.");
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
            CX_THROW_DEF(GLADException);
        }

        // Create the renderer
        m_Renderer = std::make_unique<Renderer>(m_Width, m_Height);

        m_Renderer->SetClearColour(0.2f, 0.2f, 0.2f, 1.0f);
        m_Renderer->Clear();
        SDL_GL_SwapWindow(m_SdlWindow);

        // Blending
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glViewport(0, 0, m_Width, m_Height);

        // MISC
        // TODO: When in editor mode, the initial window size is 0 by 0 which causes
        // gl to crash when creating the framebuffer for texture picking.
        // m_TexPick = std::make_unique<TexturePicking>(GetWidth(), GetHeight());
        // m_BatcherShader = std::make_unique<Shader>("texture2d.glsl");

        // Initialize subsystems
        KeyHandler::Init();
        MouseHandler::Init();
        DebugDraw::Init();
        Resources::Init();

        // Initialize scene
        ChangeScene(0);

        // Add the event watcher and call update
        // SDL_AddEventWatch(SDLEventFilterWatch_Bootstrap, this);
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
        while (SDL_PollEvent(&m_SdlEvent))
        {
            ImGui_ImplSDL2_ProcessEvent(&m_SdlEvent);
            switch (m_SdlEvent.type)
            {
                using MouseEvent = MouseHandler::MouseEvent;
                using KeyEvent   = KeyHandler::KeyEvent;

                case SDL_QUIT: {
                    Application::Get().Stop();
                    break;
                }
                case SDL_WINDOWEVENT: {
                    switch (m_SdlEvent.window.event)
                    {
                        case SDL_WINDOWEVENT_RESIZED: {
                            // #ifdef CX_MODE_STANDALONE // Process resize events only when in standalone mode.
                            i32 width  = m_SdlEvent.window.data1;
                            i32 height = m_SdlEvent.window.data2;
                            OnWindowResize_Event(width, height);
                            // #endif
                            break;
                        }
                    }
                }
                case SDL_MOUSEMOTION: {
                    MouseEvent mouse_event;
                    mouse_event.x = m_SdlEvent.motion.x;
                    mouse_event.y = m_SdlEvent.motion.y;
                    MouseHandler::OnMouseMove_Event(mouse_event);
                    break;
                }
                case SDL_MOUSEBUTTONDOWN: {
                    MouseEvent mouse_event;
                    mouse_event.x      = m_SdlEvent.motion.x;
                    mouse_event.y      = m_SdlEvent.motion.y;
                    mouse_event.button = (u8)m_SdlEvent.button.button - 1;
                    mouse_event.clicks = m_SdlEvent.button.clicks;
                    mouse_event.action = m_SdlEvent.button.state;
                    MouseHandler::OnMouseButton_Event(mouse_event);
                    break;
                }
                case SDL_MOUSEBUTTONUP: {
                    MouseEvent mouse_event;
                    mouse_event.x      = m_SdlEvent.motion.x;
                    mouse_event.y      = m_SdlEvent.motion.y;
                    mouse_event.button = (u8)m_SdlEvent.button.button - 1;
                    mouse_event.clicks = m_SdlEvent.button.clicks;
                    mouse_event.action = m_SdlEvent.button.state;
                    MouseHandler::OnMouseButton_Event(mouse_event);
                    break;
                }
                case SDL_MOUSEWHEEL: {
                    MouseEvent mouse_event;
                    mouse_event.x         = m_SdlEvent.wheel.mouseX;
                    mouse_event.y         = m_SdlEvent.wheel.mouseY;
                    mouse_event.scrollX   = m_SdlEvent.wheel.x;
                    mouse_event.scrollY   = m_SdlEvent.wheel.y;
                    mouse_event.scrollDir = m_SdlEvent.wheel.direction;
                    MouseHandler::OnMouseScroll_Event(mouse_event);
                    break;
                }
                case SDL_KEYDOWN: {
                    KeyEvent key_event;
                    key_event.action = m_SdlEvent.key.state;
                    key_event.key    = Key(m_SdlEvent.key.keysym.sym);
                    key_event.repeat = m_SdlEvent.key.repeat;
                    KeyHandler::OnKeyPress_Event(key_event);
                    break;
                }
                case SDL_KEYUP: {
                    KeyEvent key_event;
                    key_event.action = m_SdlEvent.key.state;
                    key_event.key    = Key(m_SdlEvent.key.keysym.sym);
                    key_event.repeat = m_SdlEvent.key.repeat;
                    KeyHandler::OnKeyPress_Event(key_event);
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

        m_CurrentScene->Update(delta_time);
        m_CurrentScene->Render(delta_time);

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

        // SDL_SetWindowTitle(m_SdlWindow, fmt::format("ms: {} fps: {}", delta_time, m_Fps).c_str());
        if (m_FrameCap > 0)
            SDL_Delay((u32)(1.0f / (f32)m_FrameCap * 1000.0f));
        m_FrameCount++;
        SDL_GL_SwapWindow(m_SdlWindow);
    }

    void Window::ChangeScene(const i32 sceneId)
    {
        switch (sceneId)
        {
            case 0:
                m_CurrentScene = std::make_unique<EditorScene>(m_Renderer.get(), m_Width, m_Height);
                m_CurrentScene->Init();
                break;
            case 1:
                m_CurrentScene = std::make_unique<LevelScene>(m_Renderer.get(), m_Width, m_Height);
                m_CurrentScene->Init();
                break;
            default: break;
        }
    }

    void Window::OnWindowResize_Event(const i32 newWidth, const i32 newHeight)
    {
        if (newWidth == 0 || newHeight == 0)
            return;
        glViewport(0, 0, newWidth, newHeight);
        m_CurrentScene->OnWindowResize_Event(newWidth, newHeight);
    }
} // namespace codex
