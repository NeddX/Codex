#ifndef CODEX_CORE_ENTRY_POINT_H
#define CODEX_CORE_ENTRY_POINT_H

#include <sdafx.h>

#include "Application.h"

extern codex::Application* codex::CreateApplication(ApplicationCLIArgs args);

class ImGuiContext;
extern ImGuiContext* GImGui;

int main(int argc, char** argv)
{
    try
    {
        auto app = codex::CreateApplication({ argc, argv });
        GImGui = codex::Application::GetImGuiLayer()->GetImGuiContext();
        app->OnInit();
        app->Run();
        delete app;
    }
    catch (const codex::CodexException& ex)
    {
		lgx::Log(lgx::Level::Fatal, ex.to_string());
        std::exit(EXIT_FAILURE);
    }
    return 0;
}

#endif // CODEX_CORE_ENTRY_POINT_H
