#define _CRTDBG_MAP_ALLOC
#include <iostream>
#include <Codex.h>
#include <crtdbg.h>
#include <memory>

int main(int argc, char* argv[])
{
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
    codex::Window::Properties info;
	info.vsync = true;
	info.frameCap = 0;
	info.width = 800;
	info.height = 600;
	codex::Window* window = codex::Window::Get();
	window->Init(info);
	window->EngineThread();
    window->Destroy();
	return 0;
}
