#ifndef CODEX_CORE_ENTRY_POINT_H
#define CODEX_CORE_ENTRY_POINT_H

#include <sdafx.h>

#include "Application.h"

extern codex::Application* codex::CreateApplication(const ApplicationCLIArgs args);

// On OSX global variables defined with extern are not inter-process when linking dynamically
// and ImGui defines a global variable called GImGui that holds a pointer
// to the current active context. Since ImGui is linked into CodexEngine and CodexEditor loads CodexEngine
// at runtime, the global variable defined by CodexEngine's imgui_internal.h and CodexEditor's imgui_internal.h
// differ.
#ifdef CX_PLATFORM_OSX
class ImGuiContext;
extern ImGuiContext* GImGui;
#endif

int main(int argc, char** argv)
{
    auto app = codex::CreateApplication({ argc, argv });

#ifdef CX_PLATFORM_OSX
    GImGui = codex::Application::GetImGuiLayer()->GetImGuiContext();
#endif
    app->Run();
    delete app;
    return 0;
}

#endif // CODEX_CORE_ENTRY_POINT_H
