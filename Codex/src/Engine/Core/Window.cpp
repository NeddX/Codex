#include "Window.h"
#include "../Renderer/DebugDraw.h"

namespace codex {	
	Window::Window()
	{

	}

	Window::~Window()
	{
		m_Running = false;
		DebugDraw::Destroy();
		KeyHandler::Destroy();
		MouseHandler::Destroy();
		Resources::Destroy();
		SDL_GL_DeleteContext(m_GlContext);
		SDL_DestroyWindow(m_SdlWindow);
		SDL_Quit();
	}

	void Window::Init(const Properties windowInfo, const void* nativeWindow)
	{
		m_NativeWindow	= nativeWindow;
		m_Title			= windowInfo.title;
		m_Width			= windowInfo.width;
		m_Height		= windowInfo.height;
		m_PosX			= windowInfo.posX;
		m_PosY			= windowInfo.posY;
		m_FrameCap		= windowInfo.frameCap;
		m_Flags			= windowInfo.flags;
		m_Running		= true;
		m_Fps			= 0;
		m_FrameCount	= 0;
		m_Tp1			= std::chrono::system_clock::now();
		m_Tp2			= m_Tp1;

		// Initialize SDL and OpenGL
		if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
		{
			SDLThrowError(__LINE__, "ERROR: FAILED TO INITIALIZE SDL!");
			return;
		}
		SDLCheckError(__LINE__);
		
		// Tell SDL to use OpenGL 3.3.0 Core
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

		// Create SDL Window
		if (m_NativeWindow) {
			m_SdlWindow = SDL_CreateWindowFrom(m_NativeWindow, m_Flags | SDL_WINDOW_OPENGL);
			printf("Creating SDL Window from a Native Window. Window Address: %p\n", m_NativeWindow);
		}
		else 
			m_SdlWindow = SDL_CreateWindow(m_Title.c_str(), m_PosX, m_PosY, m_Width, m_Height, m_Flags | SDL_WINDOW_OPENGL);

		if (!m_SdlWindow)
		{
			SDLThrowError(__LINE__, "ERROR: FAILED TO CREATE SDL WINDOW!");
		}
		SDLCheckError(__LINE__);

		// Create OpenGL context from SDL Window
		m_GlContext = SDL_GL_CreateContext(m_SdlWindow); // fails here
		if (!m_GlContext)
		{
			SDLThrowError(__LINE__, "ERROR: FAILED TO CREATE AN OPENGL CONTEXT FROM SDL WINDOW!");
		}
		SDLCheckError(__LINE__);

		// Enable VSync
		SDL_GL_SetSwapInterval((windowInfo.vsync) ? 1 : 0);

		// Initialize GLAD
		if (gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
		{
			gladLoadGL();
			fmt::println("Glad loaded.");
			gladLoadGLLoader(SDL_GL_GetProcAddress);
			printf("Vendor:   %s\n", glGetString(GL_VENDOR));
			printf("Renderer: %s\n", glGetString(GL_RENDERER));
			printf("Version:  %s\n", glGetString(GL_VERSION));
		}
		else 
		{
			fmt::println("FAILED TO INITIALIZE GLAD!");
			std::exit(-1);
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
		//m_TexPick = std::make_unique<TexturePicking>(GetWidth(), GetHeight());
		//m_BatcherShader = std::make_unique<Shader>("texture2d.glsl");

		// Initialize subsystems
		KeyHandler::Init();
		MouseHandler::Init();
		DebugDraw::Init();
		Resources::Init();

		// Initialize scene
		ChangeScene(0);

		// Add the event watcher and call update
		SDL_AddEventWatch(SDLEventFilterWatch_Bootstrap, this);
		fmt::println("Window subsystem initialized.");
	}

	void Window::SDLCheckError(const i32 line)
	{
#ifdef CODEX_CONF_DEBUG
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

	i32 Window::SDLEventFilterWatch(SDL_Event* event)
	{
		if (!m_Running) return 0;

		switch (event->type)
		{
			case SDL_WINDOWEVENT:
			{

				switch (event->window.event)
				{
					case SDL_WINDOWEVENT_RESIZED:
					{
#ifdef CX_MODE_STANDALONE // Process resize events only when in standalone mode.
						i32 width = event->window.data1;
						i32 height = event->window.data2;
						OnWindowResize_Event(width, height);
#endif
					}
				}
			}
			case SDL_MOUSEMOTION:
			{
				MouseHandler::MouseEvent mouse_event;
				mouse_event.x = event->motion.x;
				mouse_event.y = event->motion.y;
				MouseHandler::OnMouseMove_Event(mouse_event);
				break;
			}
			case SDL_MOUSEBUTTONDOWN:
			{
				MouseHandler::MouseEvent mouse_event;
				mouse_event.x = event->motion.x;
				mouse_event.y = event->motion.y;
				mouse_event.button = (u8) event->button.button - 1;
				mouse_event.clicks = event->button.clicks;
				mouse_event.action = event->button.state;	
				MouseHandler::OnMouseButton_Event(mouse_event);
				break;
			}
			case SDL_MOUSEBUTTONUP:
			{
				MouseHandler::MouseEvent mouse_event;
				mouse_event.x = event->motion.x;
				mouse_event.y = event->motion.y;
				mouse_event.button = (u8) event->button.button - 1;
				mouse_event.clicks = event->button.clicks;
				mouse_event.action = event->button.state;
				MouseHandler::OnMouseButton_Event(mouse_event);
				break;
			}
			case SDL_MOUSEWHEEL:
			{
				MouseHandler::MouseEvent mouse_event;
				mouse_event.x = event->wheel.mouseX;
				mouse_event.y = event->wheel.mouseY;
				mouse_event.scrollX = event->wheel.x;
				mouse_event.scrollY = event->wheel.y;
				mouse_event.scrollDir = event->wheel.direction;
				MouseHandler::OnMouseScroll_Event(mouse_event);
				break;
			}
			case SDL_KEYDOWN:
			{
				KeyHandler::KeyEvent key_event;
				key_event.action = event->key.state;
				key_event.key = event->key.keysym.sym;
				key_event.repeat = event->key.repeat;
				KeyHandler::OnKeyPress_Event(key_event);
				break;
			}
			case SDL_KEYUP:
			{
				KeyHandler::KeyEvent key_event;
				key_event.action = event->key.state;
				key_event.key = event->key.keysym.sym;
				key_event.repeat = event->key.repeat;
				KeyHandler::OnKeyPress_Event(key_event);
				break;
			}
		}
		return 0;
	}

	void Window::EngineThread()
	{
		while (m_Running) Update();
	}

	void Window::Update()
	{
		//static mgl::FrameBufferProperties props(GetWidth(), GetHeight(), { mgl::TextureFormat::RGBA8, mgl::TextureFormat::RedInt32 });
		//static mgl::FrameBuffer* fb = new mgl::FrameBuffer(props);
		static f32 delta_time = -1.0f;

		m_Renderer->SetClearColour(0.2f, 0.2f, 0.2f, 1.0f);
		m_Renderer->Clear();

#ifdef CODEX_CONF_DEBUG
		DebugDraw::Begin();
#endif

		// Handle events
		while (SDL_PollEvent(&m_SdlEvent))
		{
			switch (m_SdlEvent.type)
			{
			case SDL_QUIT:
				m_Running = false;
				break;
			default:
				break;
			}
		}

		// Update scene
#ifdef CODEX_CONF_DEBUG
		DebugDraw::Render();
#endif
		if (delta_time != -1.0f)
		{
			m_CurrentScene->Update(delta_time);
			m_CurrentScene->Render(delta_time);
		}
		
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

		// Exit
		if (KeyHandler::IsKeyDown(SDLK_ESCAPE)) m_Running = false;

		m_Tp2 = std::chrono::system_clock::now();
		delta_time = std::chrono::duration<f32>(m_Tp2 - m_Tp1).count();
		m_Tp1 = m_Tp2;
		m_Fps = (u32)(1.0f / delta_time);
		//SDL_SetWindowTitle(m_SdlWindow, fmt::format("ms: {} fps: {}", delta_time, m_Fps).c_str());
		if (m_FrameCap > 0) SDL_Delay((u32)(1.0f / (f32)m_FrameCap * 1000.0f));
		m_FrameCount++;
		SDL_GL_SwapWindow(m_SdlWindow);
	}

	void Window::ChangeScene(const i32 sceneId)
	{
		switch (sceneId)
		{
			case 0:
				m_CurrentScene = std::make_unique<EditorScene>(m_Instance->m_Renderer.get(), m_Instance->m_Width, m_Instance->m_Height);
				m_CurrentScene->Init();
				break;
			case 1:
				m_CurrentScene = std::make_unique<LevelScene>(m_Instance->m_Renderer.get(), m_Instance->m_Width, m_Instance->m_Height);
				m_CurrentScene->Init();
				break;
			default:
				break;
		}
	}

	void Window::Destroy()
	{
		if (m_Instance)
		{
			delete m_Instance;
			m_Instance = nullptr;
			fmt::println("Window sub system disposed");
		}
	}

	void Window::OnWindowResize_Event(const u32 newWidth, const u32 newHeight)
	{
		if (newWidth == 0 || newHeight == 0) return;
		glViewport(0, 0, newWidth, newHeight);
		m_CurrentScene->OnWindowResize_Event(newWidth, newHeight);
	}
}
