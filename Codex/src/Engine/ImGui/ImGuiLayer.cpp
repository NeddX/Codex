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
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

        f32 font_size = 18.0f;

        ImGui::StyleColorsDark();

        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding              = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        SetDarkThemeColours();

        auto& app        = Application::Get();
        auto* window     = Application::Get().GetWindow().GetNativeWindow();
        auto* gl_context = Application::Get().GetWindow().GetGlContext();

        ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
        ImGui_ImplOpenGL3_Init("#version 330");
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

    void ImGuiLayer::Update(const f32 deltaTime)
    {
        Begin();
        End();
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

        // Rendering
        static bool show = true;
        ImGui::ShowDemoWindow(&show);
        ImGui::Render();
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
