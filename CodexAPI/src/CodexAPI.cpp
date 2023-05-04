#include "CodexAPI.h"

extern Codex::Window* g_WindowInstance;

void Init()
{
	if (g_WindowInstance == nullptr)
		g_WindowInstance = Codex::Window::CreateWindow();
}

void CreateWindow(const Codex::Window::Properties& properties, const void* nativeWindow)
{
	g_WindowInstance->Init(properties, nativeWindow);
}

void Destroy()
{
	g_WindowInstance->Destroy();
}

void StartEngineThread()
{
	g_WindowInstance->Update();
}

void Update()
{
	g_WindowInstance->ManualUpdate();
}

void ResizeViewport(int newWidth, int newHeight)
{
	g_WindowInstance->OnWindowResize_Event(newWidth, newHeight);
}

Codex::Scene* GetCurrentScene()
{
	return nullptr;
}

void ChangeScene(int sceneID)
{

}

uint32_t CreateEntity(EntityDescriptor* desc)
{
	return g_WindowInstance->GetCurrentScene()->CreateEntity(desc->tag.tag).GetID();
}

void RemoveEntity(uint32_t id)
{
	g_WindowInstance->GetCurrentScene()->RemoveEntity(id);
}
