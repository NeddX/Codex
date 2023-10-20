#ifndef CODEX_CORE_WINDOW_H
#define CODEX_CORE_WINDOW_H

#include <sdafx.h>

#include "CommonDef.h"
#include "MouseHandler.h"
#include "KeyHandler.h"
#include "../Scene/Scene.h"
#include "../Scene/LevelScene.h"
#include "../Scene/EditorScene.h"
#include "../Renderer/Renderer.h"
#include "../Renderer/TexturePicking.h"

namespace codex {
	// Forward declerations
	class Application;

	class Window
	{
		friend class Application;

	private:
		std::string m_Title;
		u32 m_Width, m_Height;
		u32 m_PosX, m_PosY;
		u32 m_Flags;
		bool m_Running;
		u32 m_Fps, m_FrameCount, m_FrameCap;
		std::chrono::system_clock::time_point m_Tp1, m_Tp2;
		const void* m_NativeWindow;
		std::unique_ptr<Scene> m_CurrentScene;
		std::unique_ptr<Renderer> m_Renderer;
		SDL_Window* m_SdlWindow;
		SDL_GLContext m_GlContext;
		SDL_Event m_SdlEvent;

	private:
		Window();
		Window(const Window& other) = delete;
		Window& operator=(const Window& other) = delete;
		Window(Window&& other) noexcept = delete;
		Window& operator=(Window&& other) noexcept = delete;
		~Window();

	public:
		inline u32 GetWidth() const
			{ return m_Width; }
		inline u32 GetHeight() const
			{ return m_Height; }

	public:
		struct Properties
		{
			const char* title = "Codex - Window";
			u32			width = 1280;
			u32			height = 720;
			u32			posX = SDL_WINDOWPOS_CENTERED;
			u32			posY = SDL_WINDOWPOS_CENTERED;
			u32			frameCap = 300;
			u32	        flags = SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;
			bool		vsync = true;
			bool		borderless = false;

		public:
			Properties()
			{

			}
		};

	public:
		void Init(const Properties windowInfo = Properties(), const void* nativeWindow = nullptr);
		void EngineThread();
		void Update();
		i32 SDLEventFilterWatch(SDL_Event* event);
		void ChangeScene(const i32 sceneId);
		void SDLCheckError(const i32 line = -1);
		void SDLThrowError(const i32 line, const std::string_view errorMessage);
		void OnWindowResize_Event(const u32 newWidth, const u32 newHeight);

	private:
		static inline i32 SDLEventFilterWatch_Bootstrap(void* object, SDL_Event* event)
			{ return ((Window*)object)->SDLEventFilterWatch(event); }
	};
}

#endif // CODEX_CORE_WINDOW_H
