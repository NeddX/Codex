#ifndef CODEX_CORE_WINDOW_H
#define CODEX_CORE_WINDOW_H

#include <sdafx.h>

#include "CommonDef.h"
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
		void* operator new(usize size)
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
		i32 m_Width, m_Height;
		i32 m_PosX, m_PosY;
		u32 m_Flags;
		bool m_Running;
		u32 m_Fps, m_FrameCount, m_FrameCap;
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
		inline u32 GetWidth() const
			{ return m_Width; }
		inline u32 GetHeight() const
			{ return m_Height; }

	public:
		struct Properties
		{
			const char* title = "Codex - Window";
			i32			width = 1280;
			i32			height = 720;
			i32			posX = SDL_WINDOWPOS_CENTERED;
			i32			posY = SDL_WINDOWPOS_CENTERED;
			i32			frameCap = 300;
			u32	flags = SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;
			bool		vsync = true;
			bool		borderless = false;
		public:
			Properties()
			{

			}
		};

	public:
		static Window* Get();
		static i32 SDLEventFilterWatch(void* object, SDL_Event* event);
		static void ChangeScene(const i32 sceneId);

	private:
		void SDLCheckError(const i32 line = -1);
		void SDLThrowError(const i32 line, const std::string_view errorMessage);

	public:
		void Init(const Properties windowInfo = Properties(), const void* nativeWindow = nullptr);
		void EngineThread();
		inline void Update();
		void Destroy();

	public:
		static void OnWindowResize_Event(const i32 newWidth, const i32 newHeight);
	};
}

#endif // CODEX_CORE_WINDOW_H
