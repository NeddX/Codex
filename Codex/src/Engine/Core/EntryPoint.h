#ifndef CODEX_CORE_ENTRY_POINT_H
#define CODEX_CORE_ENTRY_POINT_H

#include <sdafx.h>

#include "Application.h"

extern codex::Application* codex::CreateApplication(ApplicationCLIArgs args);

class ImGuiContext;
extern ImGuiContext* GImGui;

int main(int argc, char** argv)
{
    auto app = codex::CreateApplication({ argc, argv });
    GImGui = codex::Application::GetImGuiLayer()->GetImGuiContext();
    app->Init();
    app->Run();
    delete app;
    return 0;
}

#endif // CODEX_CORE_ENTRY_POINT_H
