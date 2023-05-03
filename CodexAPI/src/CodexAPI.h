#ifndef CLIAPI_H
#define CLIAPI_H

#if defined(_WIN32) || defined(__CYGWIN__)
#   define CFX_EXPORT __declspec(dllexport)
#else
#   define CFX_EXPORT __attribute__((visibility("default")))
#endif

#include <Codex.h>
#include <cstdint>

#ifdef __cplusplus
extern "C" {
#endif
	 
	CFX_EXPORT Codex::Window* GetInstance();
	CFX_EXPORT Codex::Window* CreateChildWindow(
		Codex::Window* inst, 
		const Codex::Window::Properties& properties,
		const void* parentHandle,
		void*& handle);
	CFX_EXPORT void CreateWindow(Codex::Window* inst, const Codex::Window::Properties& properties, const void* nativeWindow = nullptr);
	CFX_EXPORT void Dispose(Codex::Window* inst);
	CFX_EXPORT void StartEngineThread(Codex::Window* inst);
	CFX_EXPORT void Update(Codex::Window* inst);
	CFX_EXPORT void ResizeViewport(Codex::Window* inst, int newWidth, int newHeight);

#ifdef __cplusplus
}
#endif

#endif // CLIAPI_H