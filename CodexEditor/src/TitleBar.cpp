#include "TitleBar.h"

#include <EditorApplication.h>

namespace codex::editor {
    TitleBar::TitleBar()
    {
        m_TitleBarIcons[0] = gfx::Texture2D(EditorApplication::GetAppDataPath() / "UI/titlebar_minimize.png");
        m_TitleBarIcons[1] = gfx::Texture2D(EditorApplication::GetAppDataPath() / "UI/titlebar_maximize.png");
        m_TitleBarIcons[2] = gfx::Texture2D(EditorApplication::GetAppDataPath() / "UI/titlebar_close.png");
    }

    TitleBar::~TitleBar()
    {
    }

    void TitleBar::OnAttach()
    {
    }

    void TitleBar::OnDetach()
    {
    }

    void TitleBar::OnUpdate(const codex::f32 deltaTime)
    {
        static auto&        win = Application::GetWindow();
        static SystemCursor cursor;
        static SystemCursor prev_cursor;
        static bool         is_dragging_window = false;

        // Window resize.
        /*
        {
            auto        pos = Input::GetMousePos();
            static bool l = false, r = false;

            if (true)
            {
                if (pos.y <= 3 || pos.y >= win.GetHeight() - 3)
                {
                    cursor = SystemCursor::VerticalResize;
                    l      = true;
                }
                else
                    l = false;
                if (pos.x <= 3 || pos.x >= win.GetWidth() - 3)
                {
                    cursor = SystemCursor::HorizontalResize;
                    r      = true;
                }
                else
                    r = false;

                if (l && r)
                {
                    cursor = SystemCursor::DiagonalLeftResize;
                }
            }

            if (Input::IsMouseDragging())
            {
                if (l || r)
                {
                    const auto delta = Input::GetMouseDelta();
                    const auto size  = win.GetSize();
                    auto       pos   = win.GetPosition();
                    win.SetPosition({ Input::GetScreenMousePos().x, pos.y });
                    win.SetSize({ size.x + (pos.x - win.GetPosition().x), size.y });
                }
            }
        }
        */

        // Window move
        // TODO: Replace the magic numbers.
        /*
        {
            static Vector2 diff{};

            auto pos       = Application::GetWindow().GetPosition();
            auto mouse_pos = Input::GetMousePos();
            if (Input::IsMouseDragging())
            {
                if (is_dragging_window)
                {
                    win.SetPosition(Input::GetScreenMousePos() - diff);
                    cursor = SystemCursor::Resize;
                }
                else if (mouse_pos.y >= 0 && mouse_pos.y <= 18)
                {
                    is_dragging_window = true;
                    diff               = Input::GetScreenMousePos() - win.GetPosition();
                }
            }
            else
            {
                is_dragging_window = false;
                cursor             = SystemCursor::Arrow;
            }
        }
        */

        if (prev_cursor != cursor)
        {
            prev_cursor = cursor;
            win.SetCursor(cursor);
        }
    }

    void TitleBar::OnImGuiRender()
    {
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;

        // Code stolen from:
        // https://github.com/StudioCherno/Walnut/blob/7e478a3828059eafabc31cb0559a31d1d83dad12/Walnut/Platform/GUI/Walnut/ApplicationGUI.cpp

        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollWithMouse;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus |
                        ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar;

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2());
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

        ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImVec4());
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4());
        ImGui::Begin("DockSpaceWindow", nullptr, window_flags);
        ImGui::PopStyleColor(2); // MenuBarBg
        ImGui::PopStyleVar(2);

        ImGui::PopStyleVar(2);

        DrawTitleBar();
        ImGui::SetCursorPosY(m_TitleBarHeight);

        auto& style          = ImGui::GetStyle();
        f32   min_win_size_x = style.WindowMinSize.x;
        ImGui::DockSpace(ImGui::GetID("MyDockspace"));

        ImGui::End();
    }

    void TitleBar::DrawTitleBar()
    {
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;

        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollWithMouse;
        window_flags |= ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar;

        const auto* viewport = ImGui::GetMainViewport();

        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(ImVec2(viewport->WorkSize.x, m_TitleBarHeight));

        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2());

        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4{ 0.01f, 0.01f, 0.01f, 1.0f });

        ImGui::Begin("##title-birren", nullptr, window_flags);
        ImGui::PopStyleVar(2);
        ImGui::PopStyleColor(1);
        // ImGui::Image(img_icon, ImVec2{ 1.0f, 1.0f });
        ImGui::SetWindowFontScale(1.0f);
        ImGui::TextColored(ImVec4{ 1.0f, 1.0f, 1.0f, 1.0f }, "Application Title");
        ImGui::SameLine();

        const auto& win          = Application::GetWindow();
        const auto  image_size   = ImVec2(16, 16);
        const auto  spacing      = 15.0f;
        const auto  window_width = ImGui::GetWindowWidth();
        const auto  pos          = window_width - (3 * (image_size.x + spacing)); // Calculate the starting position

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
        ImGui::SetCursorPosX(pos);
        if (ImGui::ImageButton((void*)(intptr)m_TitleBarIcons[0].GetGlId(), image_size, { 0, 1 }, { 1, 0 }))
            win.Minimize();
        ImGui::SameLine();

        ImGui::SetCursorPosX(pos + image_size.x + spacing);
        if (ImGui::ImageButton((void*)(intptr)m_TitleBarIcons[1].GetGlId(), image_size, { 0, 1 }, { 1, 0 }))
            win.Maximize();
        ImGui::SameLine();

        ImGui::SetCursorPosX(pos + 2 * (image_size.x + spacing));
        if (ImGui::ImageButton((void*)(intptr)m_TitleBarIcons[2].GetGlId(), image_size, { 0, 1 }, { 1, 0 }))
            Application::Get().Stop();
        ImGui::SameLine();
        ImGui::PopStyleColor(1);

        ImGui::End();
    }

    void TitleBar::OnEvent(events::Event& event)
    {
    }
} // namespace codex::editor
