#include "SceneEditorView.h"

#include <CEditor.h>
#include <ConsoleMan.h>
#include <EditorLayer.h>
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
        sys::ProcessInfo p_info;
        p_info.appName = "python.exe";
        //p_info.command                = "python.exe";
        p_info.redirectStdOut         = true;
        p_info.redirectStdErr         = true;
        p_info.redirectStdIn          = true;
        auto proc                     = sys::Process::New(p_info);
        proc->Event_OnOutDataReceived = [](const char* buffer, usize len)
        { ConsoleMan::AppendMessage(std::string(buffer, len)); };
        proc->Event_OnErrDataReceived = [](const char* buffer, usize len)
        { ConsoleMan::AppendMessage(std::string(buffer, len)); };
        proc->Launch();
        proc->WriteLine("print('nigger gaming')");

        m_Descriptor = Shared<SceneEditorDescriptor>::From(new SceneEditorDescriptor{ .scene = Box<Scene>::New() });

        m_SceneHierarchyView = Box<SceneHierarchyView>::New(m_Descriptor.AsRef());
        m_PropertiesView     = Box<PropertiesView>::New(m_Descriptor.AsRef());

        mgl::FrameBufferProperties props;
        props.attachments = { mgl::TextureFormat::RGBA8, mgl::TextureFormat::RedInt32 };

        // TODO: This is the scene render resolution so you should not hard code this.
        props.width   = 1920;
        props.height  = 1080;
        m_Framebuffer = Box<mgl::FrameBuffer>::New(props);

        // glEnable(GL_DEPTH_TEST);
        // glDepthFunc(GL_LESS);
    }

    void SceneEditorView::OnDetach()
    {
    }

    void SceneEditorView::Update(const f32 deltaTime)
    {
        auto& d = m_Descriptor;

        m_Framebuffer->Bind();
        Renderer::SetClearColour(0.2f, 0.2f, 0.2f, 1.0f);
        Renderer::Clear();

        BatchRenderer2D::Begin();
        m_Descriptor->scene->Update(deltaTime);
        BatchRenderer2D::End();

        auto [mx, my] = ImGui::GetMousePos();
        mx -= m_ViewportBounds[0].x;
        my -= m_ViewportBounds[0].y;
        const Vector2f viewport_size = m_ViewportBounds[1] - m_ViewportBounds[0];
        const i32      mouse_x       = (i32)mx;
        const i32      mouse_y       = (i32)my;

        if (Input::IsMouseDown(Mouse::LeftMouse) && mouse_x >= 0 && mouse_y >= 0 && mouse_x <= (i32)viewport_size.x &&
            mouse_y <= (i32)viewport_size.y && !m_GizmoActive)
        {
            Vector2f scale = { m_Framebuffer->GetProperties().width / viewport_size.x,
                               m_Framebuffer->GetProperties().height / viewport_size.y };
            Vector2f pos   = { mouse_x, viewport_size.y - mouse_y };
            pos *= scale;
            pos          = glm::round(pos);
            const i32 id = m_Framebuffer->ReadPixel(1, (i32)pos.x, (i32)pos.y);
            auto      e  = Entity((entt::entity)id, d->scene.Get());
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
                    auto& s                         = e.GetComponent<SpriteRendererComponent>().GetSprite();
                    d->selectedEntity.overlayColour = s.GetColour();
                    s.GetColour()                   = d->selectColour;
                }
            }
        }
        m_Framebuffer->Unbind();
    }

    void SceneEditorView::ImGuiRender()
    {
        auto& d  = m_Descriptor;
        auto& io = ImGui::GetIO();

        ImGui::SetNextWindowPos(ImVec2(0, 20), ImGuiCond_Always);

        // ImGuizmo
        ImGuizmo::BeginFrame();

        // Enable docking on the main window.
        ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

        static bool show_demo_window = true;
        ImGui::ShowDemoWindow(&show_demo_window);

        // File menu
        {
            if (ImGui::BeginMainMenuBar())
            {
                if (ImGui::BeginMenu("File"))
                {
                    // Add File menu items here
                    if (ImGui::MenuItem("Create new project", "Ctrl+N")) {}
                    if (ImGui::MenuItem("Open", "Ctrl+O"))
                    {
                        const char* filters[]{ "*.cxproj" };
                        const char* file = tinyfd_openFileDialog("Load a Project.", nullptr, 1, filters, nullptr, 0);
                        if (file)
                        {
                            if (d->selectedEntity.entity &&
                                d->selectedEntity.entity.HasComponent<SpriteRendererComponent>())
                            {
                                d->selectedEntity.entity.GetComponent<SpriteRendererComponent>()
                                    .GetSprite()
                                    .GetColour()         = d->selectedEntity.overlayColour;
                                d->selectedEntity.entity = Entity::None();
                            }
                            d->currentProjectPath = fs::path(file);
                            d->currentProjectPath = d->currentProjectPath.parent_path();

                            // NOTE: I do not like this.
                            fs::current_path(d->currentProjectPath);

                            UnloadScriptModule();
                            d->scene.Reset(new Scene());
                            Serializer::DeserializeScene(file, *d->scene);
                        }
                    }
                    if (ImGui::MenuItem("Compile scripts"))
                    {
                        UnloadScriptModule();
                        const auto files =
                            GetAllFilesWithExtensions(d->currentProjectPath / "Assets/", { ".h", ".hpp", ".hh" });
                        std::vector<Reflector> rf_files;
                        rf_files.reserve(files.size());

                        const fs::path output_path = fs::absolute(d->currentProjectPath / "int/");
                        for (const auto& f : files)
                            rf_files.emplace_back(f).EmitMetadata(output_path);
                        Reflector::EmitBaseClass(output_path, rf_files);

                        sys::ProcessInfo p_info;

#ifdef CX_PLATFORM_WINDOWS
                        p_info.command =
                            "cmake ./ -G \"Visual Studio 17\" -B builds/vs2022 && cmake --build builds/vs2022";
#elif defined(CX_PLATFORM_LINUX)
                        p_info.command = "./build.py --preset=linux-x86_64-debug";
#elif defined(CX_PLATFORM_OSX)
                        p_info.command = "./build.py --preset=linux-x86_64-debug";
#endif
                        p_info.onExit = [this](i32 exitCode)
                        {
                            LoadScriptModule();
                            ConsoleMan::AppendMessage("-- Script build finished.");
                        };
                        p_info.redirectStdOut = true;
                        p_info.redirectStdErr = true;

                        const auto redirector = [](const char* buffer, usize len)
                        { ConsoleMan::AppendMessage(std::string(buffer, len)); };

                        ConsoleMan::AppendMessage("-- Script build started.");
                        auto proc                     = sys::Process::New(std::move(p_info));
                        proc->Event_OnOutDataReceived = redirector;
                        proc->Event_OnErrDataReceived = redirector;
                        proc->Launch();
                    }
                    if (ImGui::MenuItem("Reload Script Module"))
                    {
                        auto entities = d->scene->GetAllEntitiesWithComponent<NativeBehaviourComponent>();
                        for (auto& e : entities)
                        {
                            auto& c = e.GetComponent<NativeBehaviourComponent>();
                            c.DisposeBehaviours();
                        }
                        d->scriptModule.Reset(
                            new DLib(fmt::format("{}/lib/libNBMan.dll", d->currentProjectPath.string())));
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
                            auto& c = e.GetComponent<NativeBehaviourComponent>();
                            c.DisposeBehaviours();
                        }
                        d->scriptModule.Reset(nullptr);
                    }
                    if (ImGui::MenuItem("Save", "Ctrl+S"))
                    {
                        // Handle the "Save" action
                        static const char* save_dir = nullptr;
                        if (!save_dir)
                        {
                            const char* filter_patterns[] = { "*.cxproj" };
                            save_dir =
                                tinyfd_saveFileDialog("Save Project", "default.cxproj", 1, filter_patterns, NULL);
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
                                d->selectedEntity.entity.GetComponent<SpriteRendererComponent>()
                                    .GetSprite()
                                    .GetColour()         = d->selectedEntity.overlayColour;
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
        }

        // Engine viewport
        {
            auto& camera = EditorLayer::GetCamera();

            ImGui::Begin("Viewport");
            const auto viewport_min_region = ImGui::GetWindowContentRegionMin();
            const auto viewport_max_region = ImGui::GetWindowContentRegionMax();
            const auto viewport_offset     = ImGui::GetWindowPos();
            m_ViewportBounds[0]            = { viewport_min_region.x + viewport_offset.x,
                                               viewport_min_region.y + viewport_offset.y };
            m_ViewportBounds[1]            = { viewport_max_region.x + viewport_offset.x,
                                               viewport_max_region.y + viewport_offset.y };

            static ImVec2 viewport_window_size;
            ImVec2        current_viewport_window_size = ImGui::GetContentRegionAvail();
            if (viewport_window_size.x != current_viewport_window_size.x ||
                viewport_window_size.y != current_viewport_window_size.y)
            {
                viewport_window_size = current_viewport_window_size;
                camera.SetWidth((i32)viewport_window_size.x);
                camera.SetHeight((i32)viewport_window_size.y);
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
                    auto proj_mat = camera.GetProjectionMatrix();
                    auto view_mat = camera.GetViewMatrix();

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
        auto& d         = m_Descriptor;
        d->scriptModule = Box<DLib>::New(fmt::format("{}/lib/libNBMan.dll", d->currentProjectPath.string()));
        if (d->scriptModule)
            fmt::println("Script module loaded.");
        else
            fmt::println("Failed to load script module.");
    }

    void SceneEditorView::UnloadScriptModule()
    {
        auto& d = m_Descriptor;
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
