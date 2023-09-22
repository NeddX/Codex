#ifndef CODEX_CORE_WINDOW_H
#define CODEX_CORE_WINDOW_H

#include <sdafx.h>

#include "Constants.h"
#include "Memory.h"
#include "MouseHandler.h"
#include "KeyHandler.h"
#include "Scene.h"
#include "EditorScene.h"
#include "LevelScene.h"
#include "../Renderer/Renderer.h"
#include "../Renderer/TexturePicking.h"

namespace Codex {
	class Window
	{
#ifdef CX_DEBUG_CUSTOM_ALLOCATORS
	public:
		void* operator new(size_t size)
		{
			void* ptr = std::malloc(size);
			fmt::println("[Memory] :: Allocated memory.\n\tFile: {}\n\tLine: {}\n\tSize: {}\n\tAddress: {}",
				__FILE__, __LINE__, size, ptr);
			return ptr;
		}
		void operator delete(void* ptr)
		{
			fmt::println("[Memory] :: Deallocated memory.\n\tFile: {}\n\tLine: {}\n\tAddress: {}", __FILE__, __LINE__, ptr);
			std::free(ptr);
		}
#endif

	private:
		const char* m_Title;
		int m_Width, m_Height;
		int m_PosX, m_PosY;
		uint32_t m_Flags;
		bool m_Running;
		uint32_t m_Fps, m_FrameCount, m_FrameCap;
		std::chrono::system_clock::time_point m_Tp1, m_Tp2;
		const void* m_NativeWindow;
		std::unique_ptr<Scene> m_CurrentScene;
		std::unique_ptr<Renderer> m_Renderer;

	private:
		static Window* m_Instance;
		static SDL_Window* m_SdlWindow;
		static SDL_GLContext m_GlContext;
		static SDL_Event m_SdlEvent;

	private:
		Window();
		~Window();

	public:
		inline Scene* GetCurrentScene() const
			{ return m_Instance->m_CurrentScene.get(); }
		inline uint32_t GetWidth() const
			{ return m_Width; }
		inline uint32_t GetHeight() const
			{ return m_Height; }

	public:
		struct Properties
		{
			const char* title = "Codex - Window";
			int			width = 1280;
			int			height = 720;
			int			posX = SDL_WINDOWPOS_CENTERED;
			int			posY = SDL_WINDOWPOS_CENTERED;
			int			frameCap = 300;
			uint32_t	flags = SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;
			bool		vsync = true;
			bool		borderless = false;
		public:
			Properties()
			{

			}
		};

	public:
		static Window* Get();
		static int SDLEventFilterWatch(void* object, SDL_Event* event);
		static void ChangeScene(const int sceneId);

	private:
		void SDLCheckError(const int line = -1);
		void SDLThrowError(const int line, const std::string_view errorMessage);

	public:
		void Init(const Properties windowInfo = Properties(), const void* nativeWindow = nullptr);
		void EngineThread();
		inline void Update();
		void Destroy();

	public:
		static void OnWindowResize_Event(const int newWidth, const int newHeight);
	};
}

#endif // CODEX_CORE_WINDOW_H
