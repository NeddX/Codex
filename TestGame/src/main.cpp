#define _CRTDBG_MAP_ALLOC
#include <iostream>
#include <Codex.h>
#include <crtdbg.h>

HINSTANCE inst = GetModuleHandle(NULL);

void testFunc()
{
    /*
    const char* appTitle = "Win32 and SDL with OpenGL";
    const int winWidth = 800;
    const int winHeight = 600;

    // Register window class
    WNDCLASS wc = {};
    wc.lpfnWndProc = DefWindowProc;
    wc.hInstance = inst;
    wc.lpszClassName = appTitle;
    RegisterClass(&wc);

    // Create native Win32 window
    HWND hwnd = CreateWindowEx(
        0,
        appTitle,
        appTitle,
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT,
        winWidth, winHeight,
        NULL,
        NULL,
        inst,
        NULL
    );

    Codex::Window *window = Codex::Window::CreateWindow((void*)(hwnd));
    Codex::Window::Properties props;
    props.width = 800;
    props.height = 600;
    window->Init(props);
    window->Update();*/
}

int main(int argc, char* argv[])
{
#ifdef _DEBUG
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	//testFunc();
	Codex::Window::Properties info;
	info.vsync = false;
	info.frameCap = 0;
	info.width = 800;
	info.height = 600;
	Codex::Window* window = Codex::Window::CreateWindow();
	window->Init(info);
	window->Update();
    delete window;

    _CrtDumpMemoryLeaks();
	return 0;
}
