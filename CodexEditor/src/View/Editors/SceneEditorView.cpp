#include "SceneEditorView.h"

#include <CEditor.h>
#include <ConsoleMan.h>
#include <EditorLayer.h>
#include <tinyfiledialogs.h>

namespace codex::editor {
    namespace stdfs = std::filesystem;

    void SceneEditorView::OnAttach()
    {
        /*
        sys::ProcessInfo inf;
        inf.command        = "ls";
        inf.redirectStdOut = true;
        //    inf.redirectStdErr = true;
        inf.onExit = [](i32) { ConsoleMan::AppendMessage("Exited."); };

        const auto ldb = [](const char* buffer, usize len) { ConsoleMan::AppendMessage(std::string(buffer, len)); };

        auto handle                     = sys::Process::New(inf);
        handle->Event_OnOutDataReceived = ldb;
        handle->Event_OnErrDataReceived = ldb;

        ConsoleMan::AppendMessage("Started.");
        handle->Launch();
        */

        m_Descriptor =
            mem::Shared<SceneEditorDescriptor>::From(new SceneEditorDescriptor{ .scene = mem::Box<Scene>::New() });

        m_SceneHierarchyView = mem::Box<SceneHierarchyView>::New(m_Descriptor.AsRef());
        m_PropertiesView     = mem::Box<PropertiesView>::New(m_Descriptor.AsRef());

        mgl::FrameBufferProperties props;
        props.attachments = { mgl::TextureFormat::RGBA8, mgl::TextureFormat::RedInt32 };

        // TODO: This is the scene render resolution so you should not hard code this.
        props.width   = 1920;
        props.height  = 1080;
        m_Framebuffer = mem::Box<mgl::FrameBuffer>::New(props);

        // EditorLayer::GetCamera().SetProjectionType(scene::Camera::ProjectionType::Perspective);

        // glEnable(GL_DEPTH_TEST);
        // glDepthFunc(GL_LESS);
    }

    void SceneEditorView::OnDetach()
    {
        auto& d = m_Descriptor;
        d->scene.Reset();
    }

    void SceneEditorView::OnUpdate(const f32 deltaTime)
    {
        auto& d = m_Descriptor;

        m_Framebuffer->Bind();
        gfx::Renderer::SetClearColour(0.2f, 0.2f, 0.2f, 1.0f);
        gfx::Renderer::Clear();

        gfx::BatchRenderer2D::Begin();
        m_Descriptor->scene->OnEditorUpdate(deltaTime);
        gfx::BatchRenderer2D::End();

        auto [mx, my] = ImGui::GetMousePos();
        mx -= m_ViewportBounds[0].x;
        my -= m_ViewportBounds[0].y;
        const Vector2f viewport_size = m_ViewportBounds[1] - m_ViewportBounds[0];
        const i32      mouse_x       = (i32)mx;
        const i32      mouse_y       = (i32)my;

        // TODO: Remove.
        if (Input::IsKeyDown(Key::Left))
            --EditorLayer::GetCamera().position.x;
        if (Input::IsKeyDown(Key::Right))
            ++EditorLayer::GetCamera().position.x;
        if (Input::IsKeyDown(Key::Up))
            --EditorLayer::GetCamera().position.y;
        if (Input::IsKeyDown(Key::Down))
            ++EditorLayer::GetCamera().position.y;
        if (Input::IsKeyDown(Key::RightShift))
        {
            using enum scene::Camera::ProjectionType;
            static auto proj = Perspective;
            EditorLayer::GetCamera().SetProjectionType(proj);
            proj = (proj == Perspective) ? Orthographic : Perspective;
        }

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

    void SceneEditorView::OnImGuiRender()
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
                            d->currentProjectPath = stdfs::path(file);
                            d->currentProjectPath = d->currentProjectPath.parent_path();

                            // NOTE: I do not like this.
                            stdfs::current_path(d->currentProjectPath);

                            // TODO: Regarding to UnloadScriptModule(), LoadScriptModule() and CompileProject() here:
                            // Some projects might just not use C++ for scripting (in the future when add C#
                            // support) so make sure to not always force load the C++ script module. For now I will
                            // leave this as is because there's only C++ support.

                            d->scene.Reset(new Scene);
                            d->scriptModulePath = d->currentProjectPath / stdfs::path("lib/libNBMan.dll");
                            d->scene->LoadScriptModule(d->scriptModulePath);
                            Serializer::DeserializeScene(file, *d->scene);

                            // FIXME: Cannot unload the script module while we have attached scripts, invalidates the
                            // vptr of our class effetively invalidating the whole thing really.
                            CompileProject(); // NOTE
                        }
                    }
                    if (ImGui::MenuItem("Compile project"))
                    {
                        CompileProject();
                    }
                    if (ImGui::MenuItem("Clear build files"))
                    {
                        auto& d = m_Descriptor;
                        d->scene->UnloadScriptModule();

                        const auto files =
                            fs::GetAllFilesWithExtensions(d->currentProjectPath / "Assets/", { ".h", ".hpp", ".hh" });
                        std::vector<rf::RFScript> rf_files;
                        rf_files.reserve(files.size());

                        const auto output_path = stdfs::absolute(d->currentProjectPath / "int/");
                        for (const auto& f : files)
                            rf_files.emplace_back(f).EmitMetadata(output_path);
                        rf::RFScript::EmitBaseClass(output_path, rf_files);

                        sys::ProcessInfo p_info;

#ifdef CX_PLATFORM_WINDOWS
                        p_info.command = "cmake --preset=windows-llvm-x86_64-debug --clear";
#elif defined(CX_PLATFORM_LINUX)
                        p_info.command = "./build.py --preset=linux-x86_64-debug --clear";
#elif defined(CX_PLATFORM_OSX)
                        p_info.command = "./build.py --preset=linux-x86_64-debug --clear";
#endif
                        p_info.onExit = [this](i32 exitCode)
                        {
                            auto& d = m_Descriptor;
                            d->scene->LoadScriptModule(d->scriptModulePath); // COME BACK
                            ConsoleMan::AppendMessage("-- Clear finished.");
                        };
                        p_info.redirectStdOut = true;
                        p_info.redirectStdErr = true;

                        const auto redirector = [](const char* buffer, usize len)
                        { ConsoleMan::AppendMessage(std::string(buffer, len)); };

                        ConsoleMan::AppendMessage("-- Clear started.");
                        auto proc                     = sys::Process::New(std::move(p_info));
                        proc->Event_OnOutDataReceived = redirector;
                        proc->Event_OnErrDataReceived = redirector;
                        proc->Launch();
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
            ImGui::Text("Batch count: %zu", gfx::BatchRenderer2D::GeBatchCount());
            ImGui::Text("Total quad count: %zu", gfx::BatchRenderer2D::GetQuadCount());
            ImGui::End();
        }

        // Scene hierarchy panel
        m_SceneHierarchyView->OnImGuiRender();
        m_PropertiesView->OnImGuiRender();
    }

    void SceneEditorView::OnEvent(events::Event& e)
    {
        events::EventDispatcher d{ e };
        d.Dispatch<events::KeyDownEvent>(BindEventDelegate(this, &SceneEditorView::OnKeyDown_Event));
        d.Dispatch<events::MouseDownEvent>(BindEventDelegate(this, &SceneEditorView::OnMouseDown_Event));
        d.Dispatch<events::MouseButtonEvent>(BindEventDelegate(this, &SceneEditorView::OnMouseButton_Event));
        d.Dispatch<events::MouseMoveEvent>(BindEventDelegate(this, &SceneEditorView::OnMouseMove_Event));
    }

    bool SceneEditorView::OnKeyDown_Event(events::KeyDownEvent& e)
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

    bool SceneEditorView::OnMouseDown_Event(events::MouseDownEvent& e)
    {
        return true;
    }

    bool SceneEditorView::OnMouseButton_Event(events::MouseButtonEvent& e)
    {
        return true;
    }

    bool SceneEditorView::OnMouseMove_Event(events::MouseMoveEvent& e)
    {
        return true;
    }

    void SceneEditorView::CompileProject()
    {
        auto& d = m_Descriptor;

        d->scripts.clear();
        d->scene->UnloadScriptModule();
        const auto files = fs::GetAllFilesWithExtensions(d->currentProjectPath / "Assets/", { ".h", ".hpp", ".hh" });
        d->scripts.reserve(files.size());

        const auto output_path = stdfs::absolute(d->currentProjectPath / "int/");
        if (!stdfs::exists(output_path))
            stdfs::create_directories(output_path);

        for (const auto& f : files)
            d->scripts.emplace_back(f).Parse().EmitMetadata(output_path);
        rf::RFScript::EmitBaseClass(output_path, d->scripts);

        sys::ProcessInfo p_info;

#ifdef CX_PLATFORM_WINDOWS
        // p_info.command = "cmake ./ -G \"Visual Studio 17\" -B builds/vs2022 && cmake --build builds/vs2022";
        p_info.command = "python3 Scripts/build.py --preset=windows-llvm-x86_64-debug --build";
#elif defined(CX_PLATFORM_LINUX)
        p_info.command = "python3 Scripts/build.py --preset=linux-x86_64-debug --build";
#elif defined(CX_PLATFORM_OSX)
        p_info.command = "python3 Scripts/build.py --preset=linux-x86_64-debug --build";
#endif
        p_info.onExit = [this](i32 exitCode)
        {
            try
            {
                auto& d = m_Descriptor;
                d->scene->LoadScriptModule(d->scriptModulePath);
            }
            catch (...)
            {
            }
            ConsoleMan::AppendMessage("-- Script build finished.");
        };

#ifndef CX_PLATFORM_UNIX
        p_info.redirectStdOut = true;
        p_info.redirectStdErr = true;
#endif

        const auto redirector = [](const char* buffer, usize len)
        { ConsoleMan::AppendMessage(std::string(buffer, len)); };

        ConsoleMan::AppendMessage("-- Script build started.");
        auto proc                     = sys::Process::New(std::move(p_info));
        proc->Event_OnOutDataReceived = redirector;
        proc->Event_OnErrDataReceived = redirector;
        proc->Launch();
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
