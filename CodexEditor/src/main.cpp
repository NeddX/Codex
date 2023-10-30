#include <Codex.h>
#include <Engine/Core/EntryPoint.h>
#include <iostream>

#include "EditorLayer.h"

class CodexEditor : public codex::Application
{
public:
    CodexEditor(const codex::ApplicationProperties& properties) : codex::Application(properties)
    {
        PushLayer(new EditorLayer());
    }

    ~CodexEditor() override {}
};

codex::Application* codex::CreateApplication(const codex::ApplicationCLIArgs args)
{
    return new CodexEditor(
        codex::ApplicationProperties{ .name             = "CodexEditor",
                                      .cwd              = "./",
                                      .args             = args,
                                      .windowProperties = {
                                          .width    = 1280,
                                          .height   = 720,
                                          .frameCap = 60,
                                          .flags    = SDL_WINDOW_MAXIMIZED | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE,
                                          .vsync    = false,
                                      } });
}
