#include <Codex.h>
#include <Engine/Core/EntryPoint.h>

#include "CEditor.h"

codex::Application* codex::CreateApplication(codex::ApplicationCLIArgs args)
{
    return new codex::editor::CEditor(
        codex::ApplicationProperties{ .name             = "CodexEditor",
                                      .cwd              = "./",
                                      .args             = std::move(args),
                                      .windowProperties = {
                                          .title    = "Codex Editor",
                                          .width    = 1280,
                                          .height   = 720,
                                          .frameCap = 60,
                                          .flags    = codex::WindowFlags::Visible | codex::WindowFlags::Resizable |
                                                   codex::WindowFlags::PositionCentre | codex::WindowFlags::Maximized,
                                          .vsync = false,
                                      } });
}
