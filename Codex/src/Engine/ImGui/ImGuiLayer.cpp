#include "ImGuiLayer.h"
#include "../Core/Application.h"

#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl2.h>
#include <imgui_internal.h>
#include <misc/cpp/imgui_stdlib.cpp>

namespace codex {
    ImGuiLayer::ImGuiLayer()
    {
    }

    ImGuiLayer::~ImGuiLayer()
    {
    }

    void ImGuiLayer::OnAttach()
    {
        auto& app           = Application::Get();
        auto& window        = Application::Get().GetWindow();
        auto* native_window = Application::Get().GetWindow().GetNativeWindow();
        auto* gl_context    = Application::Get().GetWindow().GetGlContext();

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io    = ImGui::GetIO();
        io.DisplaySize = ImVec2((f32)window.GetWidth(), (f32)window.GetHeight()); // Set to your SDL2 window size
        (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

        f32 font_size = 20.0f;
        io.Fonts->AddFontFromFileTTF("Fonts/roboto/Roboto-Bold.ttf", font_size);
        io.FontDefault = io.Fonts->AddFontFromFileTTF("Fonts/roboto/Roboto-Regular.ttf", font_size);

        ImGui::StyleColorsDark();

        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding              = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        SetDarkThemeColours();

        ImGui_ImplSDL2_InitForOpenGL(native_window, gl_context);
        ImGui_ImplOpenGL3_Init("#version 330 core");
    }

    void ImGuiLayer::OnDetach()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext();
    }

    void ImGuiLayer::OnEvent(Event& event)
    {
        if (m_Blocking)
        {
            auto& io = ImGui::GetIO();
            event.handled |= event.IsInCategory(EventCategoryMouse) & io.WantCaptureMouse;
            event.handled |= event.IsInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;
        }
    }

    void ImGuiLayer::Begin()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();
    }

    void ImGuiLayer::End()
    {
        auto& io       = ImGui::GetIO();
        auto& app      = Application::Get();
        io.DisplaySize = ImVec2((f32)app.GetWindow().GetWidth(), (f32)app.GetWindow().GetHeight());

        ImGui::Render();
        glViewport(0, 0, (i32)io.DisplaySize.x, (i32)io.DisplaySize.y);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            auto* window     = Application::GetWindow().GetNativeWindow();
            auto  gl_context = SDL_GL_GetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            SDL_GL_MakeCurrent(window, gl_context);
        }
    }

    void ImGuiLayer::SetDarkThemeColours()
    {
    }

    u32 ImGuiLayer::GetActiveWidgetID() const
    {
        return GImGui->ActiveId;
    }
} // namespace codex