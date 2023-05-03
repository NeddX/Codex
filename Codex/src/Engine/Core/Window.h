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

namespace Codex
{
	class Window : public IDisposable
	{
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

	private:
		const char* m_Title;
		int m_Width, m_Height;
		int m_PosX, m_PosY;
		uint32_t m_Flags;
		bool m_Running;
		uint32_t m_Fps, m_FrameCount, m_FrameCap;
		std::chrono::system_clock::time_point m_Tp1, m_Tp2;
		const void* m_NativeWindow;

	private:
		static Window* m_Instance;
		static SDL_Window* m_SdlWindow;
		static SDL_GLContext m_GlContext;
		static SDL_Event m_SdlEvent;
		static Scene* m_CurrentScene;
		static std::unique_ptr<Renderer> m_Renderer;
		static std::unique_ptr<EditorLayer> m_EditorLayer;

	private:
		Window();
	public:
		~Window();

	public:
		inline Scene* GetCurrentScene() const	{ return m_Instance->m_CurrentScene; }
		inline int GetWidth() const				{ return m_Width; }
		inline int GetHeight() const			{ return m_Height; }
		inline uint8_t* ReadBackBufferTo()
		{
			uint8_t* buffer = new uint8_t[m_Width * m_Height * 4];
			glReadPixels(0, 0, m_Width, m_Height, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
			return buffer;
		}

	public:
		struct Properties
		{
			const char* title = "CODEX - Window";
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
		static Window* CreateWindow();
		static int SDLEventFilterWatch(void* object, SDL_Event* event);
		static void ChangeScene(int sceneID);

	private:
		void SDLCheckError(int line = -1);
		void SDLThrowError(int line, const std::string& errorMessage);

	public:
		void Init(Properties windowInfo = Properties(), const void* nativeWindow = nullptr);
		void Update();
		void ManualUpdate();
		void Dispose() override final;

	public:
		static void OnWindowResize_Event(int newWidth, int newHeight);
	};
}

#endif // CODEX_CORE_WINDOW_H
