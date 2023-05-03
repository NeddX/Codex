#include "CodexAPI.h"

Codex::Window* GetInstance()
{
	auto* inst = Codex::Window::CreateWindow();
	return inst;
}

Codex::Window* CreateChildWindow(
	Codex::Window* inst,
	const Codex::Window::Properties& properties,
	const void* parentHandle,
	void*& handle)
{
    // Create a native win32 window and then pass the control to SDL

    HINSTANCE win_inst = GetModuleHandle(NULL);
    WNDCLASS wc = {};
    wc.lpfnWndProc = DefWindowProc;
    wc.hInstance = win_inst;
    wc.lpszClassName = properties.title;
    RegisterClass(&wc);

    // Create native Win32 window
    handle = CreateWindowEx(
        0,
        properties.title,
        properties.title,
        WS_CHILD | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT,
        properties.width, properties.height,
        (HWND)(parentHandle),
        NULL,
        win_inst,
        NULL
    );

    CreateWindow(inst, properties, handle);

	return nullptr;
}

void CreateWindow(Codex::Window* inst, const Codex::Window::Properties& properties, const void* nativeWindow)
{
	inst->Init(properties, nativeWindow);
}

void Dispose(Codex::Window* inst)
{
	inst->Dispose();
}

void StartEngineThread(Codex::Window* inst)
{
	inst->Update();
}

void Update(Codex::Window* inst)
{
    inst->ManualUpdate();
}

void ResizeViewport(Codex::Window* inst, int newWidth, int newHeight)
{
    inst->OnWindowResize_Event(newWidth, newHeight);
}
