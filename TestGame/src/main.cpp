#include <iostream>
#include <Codex.h>
#include <Engine/Core/EntryPoint.h>
#include <memory>

class TestGame : public codex::Application
{
public:
	TestGame(const codex::ApplicationProperties& properties) : codex::Application(properties)
	{

	}

	~TestGame() override
	{

	}
};

codex::Application* CreateApplication(const codex::ApplicationCLIArgs args)
{
    return new TestGame(codex::ApplicationProperties{
        .name             = "TestGame",
        .cwd              = "./",
        .args             = args,
        .windowProperties = codex::WindowProperties { .width = 800, .height = 600, .frameCap = 0, .vsync = true }
    });
}

/*
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
*/
