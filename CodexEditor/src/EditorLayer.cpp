#include "EditorLayer.h"

#include <CEditor.h>
#include <ImGuizmo.h>
#include <tinyfiledialogs.h>

namespace codex::editor {
    CODEX_USE_ALL_NAMESPACES()

    ResRef<graphics::Shader> EditorLayer::m_BatchShader = nullptr;
    mem::Box<Camera>         EditorLayer::m_Camera      = nullptr;

    ResRef<graphics::Shader> EditorLayer::GetBatchShader() noexcept
    {
        return m_BatchShader;
    }

    Camera& EditorLayer::GetCamera() noexcept
    {
        return *m_Camera;
    }

    void EditorLayer::OnAttach()
    {
        // TODO: Consider splitting these into their own separate init function.
        // Setup ImGUI.
        auto&              io             = ImGui::GetIO();
        static std::string ini_file_path  = (CEditor::GetVarAppDataPath() / "imgui.ini").string();
        static std::string font_file_path = (CEditor::GetAppDataPath() / "Fonts/roboto/Roboto-Regular.ttf").string();

        if (!fs::exists(ini_file_path))
        {
            try
            {
                fs::copy_file(CEditor::GetAppDataPath() / "imgui.ini", CEditor::GetVarAppDataPath() / "imgui.ini");
            }
            catch (const std::exception& ex)
            {
                fmt::println("[Warning] :: Failed to create variable application data folder! Some data will be lost "
                             "after closing the application.");
            }
        }

        const f32 font_size = 14.0f;
        io.IniFilename      = ini_file_path.c_str();
        io.FontDefault      = io.Fonts->AddFontFromFileTTF(font_file_path.c_str(), font_size);

        const auto width  = Application::GetWindow().GetWidth();
        const auto height = Application::GetWindow().GetHeight();
        m_BatchShader     = Resources::Load<Shader>(CEditor::GetAppDataPath() / "GL Shaders/batchRenderer.glsl");
        m_BatchShader->CompileShader({ { "CX_MAX_SLOT_COUNT", "16" } });

        // Init the renderers.
        Renderer::Init(width, height);
        BatchRenderer2D::BindShader(m_BatchShader.Get());
        m_Camera = Box<Camera>::New(width, height);

        m_BatchShader->Bind();
        m_BatchShader->SetUniformMat4f("u_View", m_Camera->GetViewMatrix());
        m_BatchShader->SetUniformMat4f("u_Proj", m_Camera->GetProjectionMatrix());
        m_BatchShader->Unbind();

        m_SceneEditorView = Box<SceneEditorView>::New();
        m_SceneEditorView->OnAttach();
    }

    void EditorLayer::OnDetach()
    {
        m_SceneEditorView->OnDetach();

        m_BatchShader.Reset();
        m_Camera.Reset();

        BatchRenderer2D::Destroy();
        Renderer::Destroy();
    }

    void EditorLayer::Update(const f32 deltaTime)
    {
        m_BatchShader->Bind();
        m_BatchShader->SetUniformMat4f("u_View", m_Camera->GetViewMatrix());
        m_BatchShader->SetUniformMat4f("u_Proj", m_Camera->GetProjectionMatrix());
        m_BatchShader->Unbind();

        m_SceneEditorView->Update(deltaTime);

    }

    void EditorLayer::ImGuiRender()
    {
        m_SceneEditorView->ImGuiRender();
    }

    void EditorLayer::OnEvent(Event& e)
    {
        // EventDispatcher d(e);
        // d.Dispatch<KeyDownEvent>(BindEventDelegate(this, &EditorLayer::OnKeyDown_Event));
    }

    bool EditorLayer::OnKeyDown_Event(KeyDownEvent& e)
    {
        /*
        switch (e.GetKey())
        {
            case Key::Num1: m_GizmoMode = GizmoMode::Translation; return true;
            case Key::Num2: m_GizmoMode = GizmoMode::Rotation; return true;
            case Key::Num3: m_GizmoMode = GizmoMode::Scale; return true;
            default: break;
        }
        */
        return false;
    }
} // namespace codex::editor
