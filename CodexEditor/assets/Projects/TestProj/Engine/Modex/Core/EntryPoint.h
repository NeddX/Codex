#ifndef CODEX_CORE_ENTRY_POINT_H
#define CODEX_CORE_ENTRY_POINT_H

#include <sdafx.h>

#include "Application.h"

extern codex::Application* codex::CreateApplication(const ApplicationCLIArgs args);

int main(int argc, char** argv)
{
    auto app = codex::CreateApplication({ argc, argv });
    app->Run();
    delete app;
    return 0;
}

#endif // CODEX_CORE_ENTRY_POINT_H
