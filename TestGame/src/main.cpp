#include <iostream>
#include <Codex.h>
#include <memory>

class TestGame : public codex::Application
{
public:
	TestGame()
	{

	}

	~TestGame()
	{

	}
};

int main(int argc, char* argv[])
{
	TestGame* app = new TestGame();
	app->Run();
	delete app;
	t.Run();
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
