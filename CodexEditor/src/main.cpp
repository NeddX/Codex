#include <Codex.h>
#include <Engine/Core/EntryPoint.h>
#include <iostream>

#include "EditorLayer.h"

class CodexEditor : public codex::Application
{
public:
    using codex::Application::Application;

public:
    void Init() override { PushLayer(new codex::editor::EditorLayer); }

public:
    ~CodexEditor() override {}
};

codex::Application* codex::CreateApplication(codex::ApplicationCLIArgs args)
{
    return new CodexEditor(codex::ApplicationProperties{
        .name             = "CodexEditor",
        .cwd              = "./",
        .args             = std::move(args),
        .windowProperties = {
            .width    = 1280,
            .height   = 720,
            .frameCap = 300,
            .flags    = codex::WindowFlags::Visible | codex::WindowFlags::Resizable | codex::WindowFlags::Borderless,
            .vsync    = false,
        } });
}
