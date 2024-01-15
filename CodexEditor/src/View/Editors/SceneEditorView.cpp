#include "SceneEditorView.h"

#include <tinyfiledialogs.h>

namespace codex::editor {
    namespace fs = std::filesystem;
    using namespace codex::events;
    using namespace codex::fs;
    using namespace codex::imgui;
    using namespace codex::mem;
    using namespace codex::graphics;
    using namespace codex::reflect;

    void SceneEditorView::OnAttach()
    {
#if defined(CX_BUILD_TYPE_DEBUG) || defined(CX_BUILD_TYPE_DEBUG)
#ifdef CX_PLATFORM_LINUX
        fmt::println("CEINST: {}", CE_INSTALL_DIR);
        m_ApplicationDataPath = fs::path(CE_INSTALL_DIR) / fs::path("share/Codex");
#elif defined(CX_PLATFORM_WINDOWS)
        m_ApplicationDataPath = fs::path(CE_INSTALL_DIR) / fs::path("bin");
#endif
#elif defined(CX_BUILD_TYPE_SHIPPING)
        m_ApplicationDataPath = fs::path(GetSpecialFolder(SpecialFolder::ApplicationFiles)) / fs::path("Codex/");
#endif
        m_VariableApplicationDataPath =
            fs::path(GetSpecialFolder(SpecialFolder::UserApplicationData)) / fs::path("Codex/");

        if (!fs::exists(m_VariableApplicationDataPath))
        {
            try
            {
                fs::create_directory(m_VariableApplicationDataPath);
            }
            catch (const std::exception& ex)
            {
                fmt::println("[Warning] :: Failed to create application data folder!");
            }
        }

        fmt::println("[Info] :: AppDataPath: '{}' VarAppDataPath: '{}'", m_ApplicationDataPath.string(),
                     m_VariableApplicationDataPath.string());

        // ImGui setup
        static std::string ini_file_path = (m_VariableApplicationDataPath / "imgui.ini").string();
        static std::string font_file_path = (m_ApplicationDataPath / "Fonts/roboto/Roboto-Regular.ttf").string();
        
        // Crash on MSVC! GImGui is initialized before the ImGui call thus
        // CodexEditor thinks there's no active ImGui context.
        auto&           io            = ImGui::GetIO();

        if (!fs::exists(ini_file_path))
        {
            try
            {
                fs::copy_file(m_ApplicationDataPath / "imgui.ini", m_VariableApplicationDataPath / "imgui.ini");
            }
            catch (const std::exception& ex)
            {
                fmt::println("[Warning] :: Failed to create variable application data folder! Some data will be lost "
                             "after closing the application.");
            }
        }

        f32 font_size  = 14.0f;
        io.IniFilename = ini_file_path.c_str();
        io.FontDefault =
            io.Fonts->AddFontFromFileTTF(font_file_path.c_str(), font_size);

        auto width    = Application::GetWindow().GetWidth();
        auto height   = Application::GetWindow().GetHeight();
        m_BatchShader = Resources::Load<Shader>(m_ApplicationDataPath / "GLShaders/batchRenderer.glsl");
        m_BatchShader->CompileShader({ { "CX_MAX_SLOT_COUNT", "16" } });
        m_Camera = Box<Camera>::New(width, height);

        m_Descriptor = Shared<SceneEditorDescriptor>::From(new SceneEditorDescriptor{ .scene = Box<Scene>::New() });

        // Panels
        // FIXME: When changing scene, panels hold the reference to the outdated scenes thus resulting in a segfault.
        // Possible solution would be to either hold reference to the unique_ptr (weird, ugly and undesirable) or
        // hold a referene to a raw pointer and manage the scene using a raw pointer, this is okay but i feel like we
        // can do better which why i am thinking about creating a custom memory management system. We will start by
        // having the following three smart pointer classes which are almost identical to their STL encounterparts:
        // Box<T>    :: Owns the underlyin objects and uses RAII to manage it. Since it holds exclusive ownership, you
        // cannot copy, only move it. Shared<T> :: Possibility of having multiple owners for the underlying object. Uses
        // a reference counter system to manage the object and disposes it when the reference counter hits 0. This is
        // also thread-safe. Ref<T>    :: Holds a possibly null reference to Box<T> or Shared<T>.
        //
        // In the future I might implement a ghetto garbage collector.
        // Managed<T> :: The underlying object is managed by the garbage collector, the object cannot be copied unless
        // the .Clone() method is used. Managed<int> obj1 = GC::New<int>(); Managed<int> obj2 = obj1;         // Both
        // obj1 and obj2 refer to the same underlying object. Managed<int> obj3 = obj1.Clone(); // obj3 holds a
        // reference to a newly made copy.
        m_SceneHierarchyView = Box<SceneHierarchyView>::New(m_Descriptor.AsRef());
        m_PropertiesView     = Box<PropertiesView>::New(m_Descriptor.AsRef());

        Renderer::Init(width, height);
        BatchRenderer2D::BindShader(m_BatchShader.get());

        mgl::FrameBufferProperties props;
        props.attachments = { mgl::TextureFormat::RGBA8, mgl::TextureFormat::RedInt32 };
        props.width       = 1280;
        props.height      = 720;
        m_Framebuffer     = Box<mgl::FrameBuffer>::New(props);

        // glEnable(GL_DEPTH_TEST);
        // glDepthFunc(GL_LESS);

        std::ifstream fs("./TestScript.h");
        std::string   src((std::istreambuf_iterator<char>(fs)), (std::istreambuf_iterator<char>()));
        TokenList     list = Lexer::Lex(src);
        Lexer::Print(list);
    }

    void SceneEditorView::OnDetach()
    {
        BatchRenderer2D::Destroy();
        Renderer::Destroy();
    }

    void SceneEditorView::Update(const f32 deltaTime)
    {
        m_BatchShader->Bind();
        m_BatchShader->SetUniformMat4f("u_View", m_Camera->GetViewMatrix());
        m_BatchShader->SetUniformMat4f("u_Proj", m_Camera->GetProjectionMatrix());

        m_Framebuffer->Bind();
        Renderer::SetClearColour(0.2f, 0.2f, 0.2f, 1.0f);
        Renderer::Clear();

        BatchRenderer2D::Begin();
        m_Descriptor->scene->Update(deltaTime);
        BatchRenderer2D::End();

        /*
        auto [mx, my] = ImGui::GetMousePos();
        mx -= m_ViewportBounds[0].x;
        my -= m_ViewportBounds[0].y;
        Vector2f viewport_size = m_ViewportBounds[1] - m_ViewportBounds[0];
        i32      mouse_x       = (i32)mx;
        i32      mouse_y       = (i32)my;

        if (Input::IsMouseDown(Mouse::LeftMouse) && mouse_x >= 0 && mouse_y >= 0 && mouse_x <= (i32)viewport_size.x &&
            mouse_y <= (i32)viewport_size.y && !m_GizmoActive)
        {
            Vector2f scale = { m_Framebuffer->GetProperties().width / viewport_size.x,
                               m_Framebuffer->GetProperties().height / viewport_size.y };
            Vector2f pos   = { mouse_x, viewport_size.y - mouse_y };
            pos *= scale;
            pos     = glm::round(pos);
            i32  id = id = m_Framebuffer->ReadPixel(1, (i32)pos.x, (i32)pos.y);
            auto e       = Entity((entt::entity)id, d->scene.get());
            if (e)
            {
                if (d->selectedEntity.entity)
                {
                    if (d->selectedEntity.entity.HasComponent<SpriteRendererComponent>())
                    {
                        d->selectedEntity.entity.GetComponent<SpriteRendererComponent>().GetSprite().GetColour() =
                            d->selectedEntity.overlayColour;
                    }
                }

                d->selectedEntity.entity = e;
                if (e.HasComponent<SpriteRendererComponent>())
                {
                    auto& s                        = e.GetComponent<SpriteRendererComponent>().GetSprite();
                    d->selectedEntity.overlayColour = s.GetColour();
                    s.GetColour()                  = m_SelectColour;
                }
            }
        }
        */
        m_Framebuffer->Unbind();
    }

    void SceneEditorView::ImGuiRender()
    {
        auto& d  = m_Descriptor;
        auto& io = ImGui::GetIO();

        // ImGuizmo
        ImGuizmo::BeginFrame();

        bool show_demo_window = true;

        // Enable docking on the main window.
        ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

        ImGui::ShowDemoWindow(&show_demo_window);

        // File menu
        {
            ImGui::BeginMainMenuBar();
            if (ImGui::BeginMenu("File"))
            {
                // Add File menu items here
                if (ImGui::MenuItem("Open", "Ctrl+O"))
                {
                    const char* filters[]{ "*.cxproj" };
                    const char* file = tinyfd_openFileDialog("Load a Project.", nullptr, 1, filters, nullptr, 0);
                    if (file)
                    {
                        if (d->selectedEntity.entity &&
                            d->selectedEntity.entity.HasComponent<SpriteRendererComponent>())
                        {
                            d->selectedEntity.entity.GetComponent<SpriteRendererComponent>().GetSprite().GetColour() =
                                d->selectedEntity.overlayColour;
                            d->selectedEntity.entity = Entity::None();
                        }
                        m_ProjectPath = std::filesystem::path(std::string{ file });
                        m_ProjectPath = m_ProjectPath.parent_path();
                        UnloadScriptModule();
                        d->scene.Reset(new Scene());
                        Serializer::DeserializeScene(file, *d->scene);
                    }
                }
                if (ImGui::MenuItem("Reload Script Module"))
                {
                    auto entities = d->scene->GetAllEntitiesWithComponent<NativeBehaviourComponent>();
                    for (auto& e : entities)
                    {
                        auto& c = e.GetComponent<NativeBehaviourComponent>();
                        // c.destroy(&c);
                    }
                    d->scriptModule.Reset(new DLib(fmt::format("{}/lib/libNBMan.dll", m_ProjectPath.string())));
                    if (d->scriptModule)
                        fmt::println("Script module reloaded.");
                    else
                        fmt::println("Failed to reload script module.");
                }
                if (ImGui::MenuItem("Unload Script Module"))
                {
                    auto entities = d->scene->GetAllEntitiesWithComponent<NativeBehaviourComponent>();
                    for (auto& e : entities)
                    {
                        auto&                         c = e.GetComponent<NativeBehaviourComponent>();
                        std::vector<std::string_view> scripts;
                        for (const auto& [k, v] : c.behaviours)
                            scripts.push_back(k);
                        for (const auto& e : scripts)
                            c.Detach(e);
                    }
                    d->scriptModule.Reset(nullptr);
                }
                if (ImGui::MenuItem("Save", "Ctrl+S"))
                {
                    // Handle the "Save" action
                    static const char* save_dir = nullptr;
                    if (!save_dir)
                    {
                        auto&       c = d->selectedEntity.entity.GetComponent<SpriteRendererComponent>().GetSprite();
                        const char* filter_patterns[] = { "*.cxproj" };
                        save_dir = tinyfd_saveFileDialog("Save Project", "default.cxproj", 1, filter_patterns, NULL);
                        if (save_dir)
                        {
                            if (d->selectedEntity.entity &&
                                d->selectedEntity.entity.HasComponent<SpriteRendererComponent>())
                            {
                                d->selectedEntity.entity.GetComponent<SpriteRendererComponent>()
                                    .GetSprite()
                                    .GetColour()         = d->selectedEntity.overlayColour;
                                d->selectedEntity.entity = Entity::None();
                            }
                            Serializer::SerializeScene(save_dir, *d->scene);
                        }
                    }
                    else
                    {
                        if (d->selectedEntity.entity &&
                            d->selectedEntity.entity.HasComponent<SpriteRendererComponent>())
                        {
                            d->selectedEntity.entity.GetComponent<SpriteRendererComponent>().GetSprite().GetColour() =
                                d->selectedEntity.overlayColour;
                            d->selectedEntity.entity = Entity::None();
                        }
                        Serializer::SerializeScene(save_dir, *d->scene);
                    }
                }
                if (ImGui::MenuItem("Exit", "Alt+F4"))
                {
                    Application::Get().Stop();
                }

                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

        // Debug viewport
        {
            ImGui::Begin("Debug viewport");
            auto viewport_min_region = ImGui::GetWindowContentRegionMin();
            auto viewport_max_region = ImGui::GetWindowContentRegionMax();
            auto viewport_offset     = ImGui::GetWindowPos();
            m_ViewportBounds[0]      = { viewport_min_region.x + viewport_offset.x,
                                         viewport_min_region.y + viewport_offset.y };
            m_ViewportBounds[1]      = { viewport_max_region.x + viewport_offset.x,
                                         viewport_max_region.y + viewport_offset.y };

            static ImVec2 viewport_window_size;
            ImVec2        current_viewport_window_size = ImGui::GetContentRegionAvail();
            if (viewport_window_size.x != current_viewport_window_size.x ||
                viewport_window_size.y != current_viewport_window_size.y)
            {
                viewport_window_size = current_viewport_window_size;
                m_Camera->SetWidth((i32)viewport_window_size.x);
                m_Camera->SetHeight((i32)viewport_window_size.y);
            }
            ImGui::Image((void*)(intptr)m_Framebuffer->GetColourAttachmentIdAt(1), current_viewport_window_size,
                         { 0, 1 }, { 1, 0 });
            ImGui::End();
        }

        // Engine viewport
        {
            ImGui::Begin("Viewport");
            static ImVec2 viewport_window_size;
            ImVec2        current_viewport_window_size = ImGui::GetContentRegionAvail();
            if (viewport_window_size.x != current_viewport_window_size.x ||
                viewport_window_size.y != current_viewport_window_size.y)
            {
                viewport_window_size = current_viewport_window_size;
                m_Camera->SetWidth((i32)viewport_window_size.x);
                m_Camera->SetHeight((i32)viewport_window_size.y);
            }
            ImGui::Image((void*)(intptr)m_Framebuffer->GetColourAttachmentIdAt(0), current_viewport_window_size,
                         { 0, 1 }, { 1, 0 });

            // Guizmo
            {
                if (d->selectedEntity.entity)
                {
                    ImGuizmo::SetOrthographic(true);
                    ImGuizmo::SetDrawlist();

                    Vector2f window_size = { ImGui::GetWindowWidth(), ImGui::GetWindowHeight() };
                    ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, window_size.x, window_size.y);

                    // Camera stuff?
                    auto proj_mat = m_Camera->GetProjectionMatrix();
                    auto view_mat = m_Camera->GetViewMatrix();

                    auto& c     = d->selectedEntity.entity.GetComponent<TransformComponent>();
                    auto  trans = c.GetTransform();

                    ImGuizmo::Manipulate(glm::value_ptr(view_mat), glm::value_ptr(proj_mat),
                                         (ImGuizmo::OPERATION)m_GizmoMode, ImGuizmo::MODE::LOCAL,
                                         glm::value_ptr(trans));

                    if (ImGuizmo::IsUsing())
                    {
                        Vector3f rot;
                        codex::math::TransformDecompose(trans, c.position, rot, c.scale);
                        c.rotation += rot;
                        m_GizmoActive = true;
                    }
                    else
                        m_GizmoActive = false;
                }
            }

            ImGui::End();
        }

        // Render info
        {
            ImGui::Begin("Render Info");
            ImGui::Text("Renderer information");
            ImGui::Text("FPS: %u", Application::GetFps());
            ImGui::Text("Delta time: %f", Application::GetDelta());
            ImGui::Text("Batch count: %zu", BatchRenderer2D::GeBatchCount());
            ImGui::Text("Total quad count: %zu", BatchRenderer2D::GetQuadCount());
            ImGui::End();
        }

        // Scene hierarchy panel
        m_SceneHierarchyView->OnImGuiRender();
        m_PropertiesView->OnImGuiRender();
    }

    void SceneEditorView::OnEvent(Event& e)
    {
        EventDispatcher d(e);
        d.Dispatch<KeyDownEvent>(BindEventDelegate(this, &SceneEditorView::OnKeyDown_Event));
    }

    bool SceneEditorView::OnKeyDown_Event(KeyDownEvent& e)
    {
        switch (e.GetKey())
        {
            using enum codex::Key;
            using enum codex::editor::GizmoMode;

            case Num1: m_GizmoMode = Translation; return true;
            case Num2: m_GizmoMode = Rotation; return true;
            case Num3: m_GizmoMode = Scale; return true;
            default: break;
        }
        return false;
    }

    void SceneEditorView::LoadScriptModule()
    {
        auto d          = m_Descriptor;
        d->scriptModule = Box<DLib>::New(fmt::format("{}/lib/libNBMan.dll", m_ProjectPath.string()));
        if (d->scriptModule)
            fmt::println("Script module loaded.");
        else
            fmt::println("Failed to load script module.");
    }

    void SceneEditorView::UnloadScriptModule()
    {
        auto d = m_Descriptor;
        if (d->scriptModule)
            d->scriptModule.Reset();
    }

    void SceneEditorView::DrawVec3Control(const char* label, Vector3f& values, const f32 columnWidth, const f32 speed,
                                          const f32 resetValue)
    {
        ImGuiIO& io        = ImGui::GetIO();
        auto     bold_font = io.Fonts->Fonts[0];

        ImGui::PushID(label);

        if (label[0] != '#' && label[1] != '#')
        {
            ImGui::Columns(2);
            ImGui::SetColumnWidth(0, columnWidth);
            ImGui::Text("%s", label);
            ImGui::NextColumn();
        }

        ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

        float  lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
        ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
        ImGui::PushFont(bold_font);
        if (ImGui::Button("X", buttonSize))
            values.x = resetValue;
        ImGui::PopFont();
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat("##X", &values.x, speed, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
        ImGui::PushFont(bold_font);
        if (ImGui::Button("Y", buttonSize))
            values.y = resetValue;
        ImGui::PopFont();
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat("##Y", &values.y, speed, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
        ImGui::PushFont(bold_font);
        if (ImGui::Button("Z", buttonSize))
            values.z = resetValue;
        ImGui::PopFont();
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat("##Z", &values.z, speed, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();

        ImGui::PopStyleVar();

        if (label[0] != '#' && label[1] != '#')
            ImGui::Columns(1);

        ImGui::PopID();
    }

    void SceneEditorView::DrawVec2Control(const char* label, Vector2f& values, const f32 columnWidth, const f32 speed,
                                          const f32 resetValue)
    {
        ImGuiIO& io        = ImGui::GetIO();
        auto     bold_font = io.Fonts->Fonts[0];

        ImGui::PushID(label);

        if (label[0] != '#' && label[1] != '#')
        {
            ImGui::Columns(2);
            ImGui::SetColumnWidth(0, columnWidth);
            ImGui::Text("%s", label);
            ImGui::NextColumn();
        }

        ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

        f32    lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
        ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
        ImGui::PushFont(bold_font);
        if (ImGui::Button("X", buttonSize))
            values.x = resetValue;
        ImGui::PopFont();
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat("##X", &values.x, speed, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
        ImGui::PushFont(bold_font);
        if (ImGui::Button("Y", buttonSize))
            values.y = resetValue;
        ImGui::PopFont();
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat("##Y", &values.y, speed, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();

        ImGui::PopStyleVar();

        if (label[0] != '#' && label[1] != '#')
            ImGui::Columns(1);

        ImGui::PopID();
    }
} // namespace codex::editor
