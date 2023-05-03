#include "Window.h"

namespace Codex
{	
	Window* Window::m_Instance							= nullptr;
	SDL_Window* Window::m_SdlWindow						= nullptr;
	SDL_GLContext Window::m_GlContext					= nullptr;
	SDL_Event Window::m_SdlEvent;
	Scene* Window::m_CurrentScene						= nullptr;
	std::unique_ptr<Renderer> Window::m_Renderer		= nullptr;
	std::unique_ptr<EditorLayer> Window::m_EditorLayer	= nullptr;

	Window::Window()
	{

	}

	Window::~Window()
	{
		delete m_CurrentScene;

		m_Running = false;
		KeyHandler::Deinit();
		MouseHandler::Deinit();
		SDL_GL_DeleteContext(m_GlContext);
		SDL_DestroyWindow(m_SdlWindow);
		SDL_Quit();
	}

	Window* Window::CreateWindow()
	{
		if (!m_Instance)
			m_Instance = new Window();
		return m_Instance;
	}

	void Window::Init(Properties windowInfo, const void* nativeWindow)
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
			m_SdlWindow = SDL_CreateWindow(m_Title, m_PosX, m_PosY, m_Width, m_Height, m_Flags | SDL_WINDOW_OPENGL);

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
			printf("OpenGL loaded\n");
			gladLoadGLLoader(SDL_GL_GetProcAddress);
			printf("Vendor:   %s\n", glGetString(GL_VENDOR));
			printf("Renderer: %s\n", glGetString(GL_RENDERER));
			printf("Version:  %s\n", glGetString(GL_VERSION));
		}
		else 
		{
			fmt::println("FAILED TO INITIALIZE GLAD!");
			exit(-1);
		}

		// Create the renderer
		m_Renderer = std::unique_ptr<Renderer>(new Renderer(m_Width, m_Height));

		// Create the editor layer TODO: Do this when in editor mode
		m_EditorLayer = std::make_unique<EditorLayer>("editor_layer", m_Renderer.get());

		// Set the background colour, draw that colour onto the back buffer and then swap the buffers
		m_Renderer->SetClearColour(0.5f, 0.5f, 0.5f, 1.0f);
		m_Renderer->Clear();
		SDL_GL_SwapWindow(m_SdlWindow);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glViewport(0, 0, m_Width, m_Height);

		fmt::println("Window successfully initialized.");

		// Initialize input
		KeyHandler::Init();
		MouseHandler::Init();

		// Initialize scene
		ChangeScene(0);

		// Add the event watcher and call update
		SDL_AddEventWatch(SDLEventFilterWatch, nullptr);
		//Update();
	}

	void Window::SDLCheckError(int line)
	{
#ifdef _DEBUG // TODO: REPLACE WITH YOUR OWN DEBUG MACRO FOR CROSS PLATFORM COMPILATION AND EXECUTION
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

	void Window::SDLThrowError(int line, const std::string& errorMessage)
	{
		fmt::println("SDL ERROR @ LINE {}: {} -> {}", line, errorMessage, SDL_GetError());
		SDL_Quit();
		exit(-1);
	}

	int Window::SDLEventFilterWatch(void* object, SDL_Event* event)
	{
		switch (event->type)
		{
			case SDL_WINDOWEVENT:
			{
				switch (event->window.event)
				{
					case SDL_WINDOWEVENT_RESIZED:
					{
#ifdef CFX_MODE_STANDALONE // Process resize events only when in standalone mode.
						int width = event->window.data1;
						int height = event->window.data2;
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
				mouse_event.button = (uint8_t) event->button.button - 1;
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
				mouse_event.button = (uint8_t) event->button.button - 1;
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

	void Window::Update()
	{
		float deltaTime = -1.0f;
		while (m_Running)
		{
			// Update
			m_Renderer->Clear();

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
			if (deltaTime != -1.0f) m_CurrentScene->Update(deltaTime);

			// Exit 
			if (KeyHandler::IsKeyDown(SDLK_ESCAPE)) m_Running = false;

			m_Tp2 = std::chrono::system_clock::now();
			deltaTime = std::chrono::duration<float>(m_Tp2 - m_Tp1).count();
			m_Tp1 = m_Tp2;
			m_Fps = (uint32_t)(1.0f / deltaTime);
			SDL_SetWindowTitle(m_SdlWindow, fmt::format("ms: {} fps: {}", deltaTime, m_Fps).c_str());
			if (m_FrameCap > 0) SDL_Delay((uint32_t)(1.0f / m_FrameCap * 1000));
			m_FrameCount++;
			SDL_GL_SwapWindow(m_SdlWindow);
		}
	}

	void Window::ManualUpdate()
	{
		static float deltaTime = -1.0f;

		// Update
		m_Renderer->Clear();
		
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
		if (deltaTime != -1.0f) m_CurrentScene->Update(deltaTime);

		// Update editor layer TODO: Implement this inside the Editor itself
		m_EditorLayer->Update(deltaTime, m_CurrentScene);

		// Exit 
		if (KeyHandler::IsKeyDown(SDLK_ESCAPE)) m_Running = false;

		m_Tp2 = std::chrono::system_clock::now();
		deltaTime = std::chrono::duration<float>(m_Tp2 - m_Tp1).count();
		m_Tp1 = m_Tp2;
		m_Fps = (uint32_t)(1.0f / deltaTime);
		//SDL_SetWindowTitle(m_SdlWindow, fmt::format("ms: {} fps: {}", deltaTime, m_Fps).c_str());
		if (m_FrameCap > 0) SDL_Delay((uint32_t)(1.0f / m_FrameCap * 1000));
		m_FrameCount++;
		SDL_GL_SwapWindow(m_SdlWindow);
	}

	void Window::ChangeScene(int sceneID)
	{
		switch (sceneID)
		{
			case 0:
				m_CurrentScene = new EditorScene(m_Renderer.get(), m_Instance->m_Width, m_Instance->m_Height);					
				m_CurrentScene->Init();
				break;
			case 1:
				m_CurrentScene = new LevelScene(m_Renderer.get(), m_Instance->m_Width, m_Instance->m_Height);
				m_CurrentScene->Init();
				break;
			default:
				break;
		}
	}

	void Window::Dispose()
	{
		delete this;
		fmt::println("Window disposed");
	}

	void Window::OnWindowResize_Event(int newWidth, int newHeight)
	{
		if (newWidth == 0 || newHeight == 0) return;
		glViewport(0, 0, newWidth, newHeight);
		m_CurrentScene->OnWindowResize_Event(newWidth, newHeight);
	}
}
