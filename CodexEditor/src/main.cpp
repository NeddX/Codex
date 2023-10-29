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
    return new CodexEditor(codex::ApplicationProperties{
        .name             = "CodexEditor",
        .cwd              = "./",
        .args             = args,
        .windowProperties = { .width = 800, .height = 600, .frameCap = 0, .vsync = false } });
}
