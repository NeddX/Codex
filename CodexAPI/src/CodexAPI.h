#ifndef CLIAPI_H
#define CLIAPI_H

#if defined(_WIN32) || defined(__CYGWIN__)
#   define CODEX_EXPORT __declspec(dllexport)
#else
#   define CODEX_EXPORT __attribute__((visibility("default")))
#endif
#define ARRAY_COUNT(x) sizeof(x) / sizeof(x[0])

#include <Codex.h>
#include <cstdint>

Codex::Window* g_WindowInstance = nullptr;

#ifdef __cplusplus
extern "C" {
#endif
	namespace
	{
		struct TransformComponent
		{
		public:
			float position[3];
			float rotation[3];
			float scale[3];
			
		public:
			Codex::TransformComponent ToNative()
			{
				Codex::TransformComponent c;
				std::memcpy(Codex::ValuePtr(c.position),	position,	sizeof(float) * ARRAY_COUNT(position));
				std::memcpy(Codex::ValuePtr(c.rotation),	rotation,	sizeof(float) * ARRAY_COUNT(rotation));
				std::memcpy(Codex::ValuePtr(c.scale),		scale,		sizeof(float) * ARRAY_COUNT(scale));
				return c;
			}
		};

		struct TagComponent
		{
		public:
			const char* tag;
		
		public:
			Codex::TagComponent ToNative()
			{
				return Codex::TagComponent(tag);
			}
		};

		struct EntityDescriptor
		{
			TransformComponent transform;
			TagComponent tag;
		};
	}

	// Window class
	CODEX_EXPORT void Init();
	CODEX_EXPORT void CreateWindow(const Codex::Window::Properties& proFperties, const void* nativeWindow = nullptr);
	CODEX_EXPORT void Destroy();
	CODEX_EXPORT void StartEngineThread();
	CODEX_EXPORT void Update();
	CODEX_EXPORT void ResizeViewport(int newWidth, int newHeight);
	CODEX_EXPORT Codex::Scene* GetCurrentScene();
	CODEX_EXPORT void ChangeScene(int sceneID);

	// Scene class
	CODEX_EXPORT uint32_t CreateEntity(EntityDescriptor* desc);
	CODEX_EXPORT void RemoveEntity(uint32_t id);

	// Entity Component System

#ifdef __cplusplus
}
#endif

#endif // CLIAPI_H