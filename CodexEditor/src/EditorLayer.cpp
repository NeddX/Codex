#include "EditorLayer.h"

#include <ImGuizmo.h>
#include <tinyfiledialogs.h>

void EditorLayer::OnAttach()
{
    auto width    = Application::GetWindow().GetWidth();
    auto height   = Application::GetWindow().GetHeight();
    m_Scene       = std::make_unique<Scene>();
    m_BatchShader = Resources::Load<Shader>("GLShaders/batchRenderer.glsl");
    m_BatchShader->CompileShader({ { "CX_MAX_SLOT_COUNT", "16" } });
    m_Camera = std::make_unique<Camera>(width, height);

    Renderer::Init(width, height);
    BatchRenderer2D::BindShader(m_BatchShader.get());

    mgl::FrameBufferProperties props;
    props.attachments = { mgl::TextureFormat::RGBA8, mgl::TextureFormat::RedInt32 };
    props.width       = 1280;
    props.height      = 720;
    m_Framebuffer     = std::make_unique<mgl::FrameBuffer>(props);

    m_SceneEditorView = std::make_unique<editor::SceneEditorView>();

    // glEnable(GL_DEPTH_TEST);
    // glDepthFunc(GL_LESS);

    std::ifstream fs("./TestScript.h");
    std::string   src((std::istreambuf_iterator<char>(fs)), (std::istreambuf_iterator<char>()));
    TokenList     list = Lexer::Lex(src);
    Lexer::Print(list);
}

void EditorLayer::OnDetach()
{
    BatchRenderer2D::Destroy();
    Renderer::Destroy();
}

void EditorLayer::Update(const f32 deltaTime)
{
    /*
    m_BatchShader->Bind();
    m_BatchShader->SetUniformMat4f("u_View", m_Camera->GetViewMatrix());
    m_BatchShader->SetUniformMat4f("u_Proj", m_Camera->GetProjectionMatrix());

    m_Framebuffer->Bind();
    Renderer::SetClearColour(0.2f, 0.2f, 0.2f, 1.0f);
    Renderer::Clear();

    BatchRenderer2D::Begin();
    m_Scene->Update(deltaTime);
    BatchRenderer2D::End();

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
        auto e       = Entity((entt::entity)id, m_Scene.get());
        if (e)
        {
            if (m_SelectedEntity.entity)
            {
                if (m_SelectedEntity.entity.HasComponent<SpriteRendererComponent>())
                {
                    m_SelectedEntity.entity.GetComponent<SpriteRendererComponent>().GetSprite().GetColour() =
                        m_SelectedEntity.overlayColour;
                }
            }

            m_SelectedEntity.entity = e;
            if (e.HasComponent<SpriteRendererComponent>())
            {
                auto& s                        = e.GetComponent<SpriteRendererComponent>().GetSprite();
                m_SelectedEntity.overlayColour = s.GetColour();
                s.GetColour()                  = m_SelectColour;
            }
        }
    }
    m_Framebuffer->Unbind();
    */
    m_SceneEditorView->Update(deltaTime);
}

void EditorLayer::ImGuiRender()
{
    auto& io = ImGui::GetIO();

    // ImGuizmo
    ImGuizmo::BeginFrame();

    bool show_demo_window = true;

    // Enable docking on the main window.
    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

    ImGui::ShowDemoWindow(&show_demo_window);

    m_SceneEditorView->ImGuiRender();
    /*
    // File menu
    {
        ImGui::BeginMainMenuBar();
        if (ImGui::BeginMenu("File"))
        {
            // Add File menu items here
            if (ImGui::MenuItem("Open", "Ctrl+O"))
            {
                const char* filters[] = { "*.cxproj" };
                const char* file      = tinyfd_openFileDialog("Load a Project.", nullptr, 1, filters, nullptr, 0);
                if (file)
                {
                    if (m_SelectedEntity.entity && m_SelectedEntity.entity.HasComponent<SpriteRendererComponent>())
                    {
                        m_SelectedEntity.entity.GetComponent<SpriteRendererComponent>().GetSprite().GetColour() =
                            m_SelectedEntity.overlayColour;
                        m_SelectedEntity.entity = Entity::None();
                    }
                    m_ProjectPath = std::filesystem::path(std::string{ file });
                    m_ProjectPath = m_ProjectPath.parent_path();
                    m_Scene.reset(new Scene());
                    Serializer::DeserializeScene(file, *m_Scene);
                    m_ScriptModule = std::make_unique<DLib>(fmt::format("{}/lib/libNBMan.dll", m_ProjectPath.string()));
                    if (m_ScriptModule)
                        fmt::println("Script module loaded.");
                    else
                        fmt::println("Failed to load script module.");
                }
            }
            if (ImGui::MenuItem("Reload Script Module"))
            {
                auto entities = m_Scene->GetAllEntitiesWithComponent<NativeBehaviourComponent>();
                for (auto& e : entities)
                {
                    auto& c = e.GetComponent<NativeBehaviourComponent>();
                    // c.destroy(&c);
                }
                m_ScriptModule.reset(new DLib(fmt::format("{}/lib/libNBMan.dll", m_ProjectPath.string())));
                if (m_ScriptModule)
                    fmt::println("Script module reloaded.");
                else
                    fmt::println("Failed to reload script module.");
            }
            if (ImGui::MenuItem("Unload Script Module"))
            {
                auto entities = m_Scene->GetAllEntitiesWithComponent<NativeBehaviourComponent>();
                for (auto& e : entities)
                {
                    auto&                         c = e.GetComponent<NativeBehaviourComponent>();
                    std::vector<std::string_view> scripts;
                    for (const auto& [k, v] : c.behaviours)
                        scripts.push_back(k);
                    for (const auto& e : scripts)
                        c.Detach(e);
                }
                m_ScriptModule.reset(nullptr);
            }
            if (ImGui::MenuItem("Save", "Ctrl+S"))
            {
                // Handle the "Save" action
                static const char* save_dir = nullptr;
                if (!save_dir)
                {
                    auto&       c = m_SelectedEntity.entity.GetComponent<SpriteRendererComponent>().GetSprite();
                    const char* filter_patterns[] = { "*.cxproj" };
                    save_dir = tinyfd_saveFileDialog("Save Project", "default.cxproj", 1, filter_patterns, NULL);
                    if (save_dir)
                    {
                        if (m_SelectedEntity.entity && m_SelectedEntity.entity.HasComponent<SpriteRendererComponent>())
                        {
                            m_SelectedEntity.entity.GetComponent<SpriteRendererComponent>().GetSprite().GetColour() =
                                m_SelectedEntity.overlayColour;
                            m_SelectedEntity.entity = Entity::None();
                        }
                        Serializer::SerializeScene(save_dir, *m_Scene);
                    }
                }
                else
                {
                    if (m_SelectedEntity.entity && m_SelectedEntity.entity.HasComponent<SpriteRendererComponent>())
                    {
                        m_SelectedEntity.entity.GetComponent<SpriteRendererComponent>().GetSprite().GetColour() =
                            m_SelectedEntity.overlayColour;
                        m_SelectedEntity.entity = Entity::None();
                    }
                    Serializer::SerializeScene(save_dir, *m_Scene);
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
        m_ViewportBounds[0] = { viewport_min_region.x + viewport_offset.x, viewport_min_region.y + viewport_offset.y };
        m_ViewportBounds[1] = { viewport_max_region.x + viewport_offset.x, viewport_max_region.y + viewport_offset.y };

        static ImVec2 viewport_window_size;
        ImVec2        current_viewport_window_size = ImGui::GetContentRegionAvail();
        if (viewport_window_size.x != current_viewport_window_size.x ||
            viewport_window_size.y != current_viewport_window_size.y)
        {
            viewport_window_size = current_viewport_window_size;
            m_Camera->SetWidth((i32)viewport_window_size.x);
            m_Camera->SetHeight((i32)viewport_window_size.y);
        }
        ImGui::Image((void*)(intptr)m_Framebuffer->GetColourAttachmentIdAt(1), current_viewport_window_size, { 0, 1 },
                     { 1, 0 });
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
        ImGui::Image((void*)(intptr)m_Framebuffer->GetColourAttachmentIdAt(0), current_viewport_window_size, { 0, 1 },
                     { 1, 0 });

        // Guizmo
        {
            if (m_SelectedEntity.entity)
            {
                ImGuizmo::SetOrthographic(true);
                ImGuizmo::SetDrawlist();

                Vector2f window_size = { ImGui::GetWindowWidth(), ImGui::GetWindowHeight() };
                ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, window_size.x, window_size.y);

                // Camera stuff?
                auto proj_mat = m_Camera->GetProjectionMatrix();
                auto view_mat = m_Camera->GetViewMatrix();

                auto& c     = m_SelectedEntity.entity.GetComponent<TransformComponent>();
                auto  trans = c.GetTransform();

                ImGuizmo::Manipulate(glm::value_ptr(view_mat), glm::value_ptr(proj_mat),
                                     (ImGuizmo::OPERATION)m_GizmoMode, ImGuizmo::MODE::LOCAL, glm::value_ptr(trans));

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
    {
        ImGui::Begin("Scene hierarchy");
        if (ImGui::Button("New entity"))
            ImGui::OpenPopup("new_entity_popup");
        if (ImGui::BeginPopup("new_entity_popup", ImGuiWindowFlags_MenuBar))
        {
            std::string name = fmt::format("Entity {}", m_Scene->GetEntityCount() + 1);
            ImGui::Text("Entity Name: ");
            ImGui::InputText("##entity_name", &name);
            ImGui::SameLine();
            if (ImGui::IsKeyPressed(ImGuiKey_Enter) || ImGui::Button("Add")) // Check for Enter key press
            {
                // Close the popup
                ImGui::CloseCurrentPopup();
                if (m_SelectedEntity.entity)
                {
                    if (m_SelectedEntity.entity.HasComponent<SpriteRendererComponent>())
                        m_SelectedEntity.entity.GetComponent<SpriteRendererComponent>().GetSprite().GetColour() =
                            m_SelectedEntity.overlayColour;
                }
                m_SelectedEntity.entity = m_Scene->CreateEntity(name);
            }
            ImGui::EndPopup();
        }

        ImGui::Text("Entities:");
        auto entities = m_Scene->GetAllEntitiesWithComponent<TagComponent>();
        for (auto& e : entities)
        {
            auto& tag_component = e.GetComponent<TagComponent>();
            if (ImGui::Selectable(tag_component.tag.c_str(), m_SelectedEntity.entity == e))
            {
                if (m_SelectedEntity.entity)
                {
                    if (m_SelectedEntity.entity.HasComponent<SpriteRendererComponent>())
                    {
                        auto& s       = m_SelectedEntity.entity.GetComponent<SpriteRendererComponent>().GetSprite();
                        s.GetColour() = m_SelectedEntity.overlayColour;
                    }
                }

                m_SelectedEntity.entity = e;

                if (m_SelectedEntity.entity)
                {
                    if (m_SelectedEntity.entity.HasComponent<SpriteRendererComponent>())
                    {
                        auto& s = m_SelectedEntity.entity.GetComponent<SpriteRendererComponent>().GetSprite();
                        m_SelectedEntity.overlayColour = s.GetColour();
                        s.GetColour()                  = m_SelectColour;
                    }
                }

                if (ImGui::IsMouseDoubleClicked(0))
                {
                    auto& t = e.GetComponent<TagComponent>();
                    ImGui::OpenPopup("new_name_popup");
                    if (ImGui::BeginPopup("new_name_popup", ImGuiWindowFlags_MenuBar))
                    {
                        ImGui::Text("New Name: ");
                        ImGui::SameLine();
                        ImGui::InputText("##entity_name", &t.tag);
                        ImGui::SameLine();
                        if (ImGui::IsKeyPressed(ImGuiKey_Enter) || ImGui::Button("Enter")) // Check for Enter key press
                            ImGui::CloseCurrentPopup();
                        ImGui::EndPopup();
                    }
                }
            }
        }
        ImGui::End();
    }
    */

    {
        ImGui::Begin("Debug info dump");

        usize i = 0;
        for (auto& b : BatchRenderer2D::GetBatches())
        {
            if (ImGui::TreeNode(fmt::format("Batch: {}", i++).c_str()))
            {
                for (usize i = 0; i < b->GetCount(); ++i)
                {
                    if (ImGui::TreeNode(fmt::format("Quad: {}", i).c_str()))
                    {
                        QuadVertex* q = b->GetQuads() + i * 4;
                        for (usize j = 0; j < 4; ++j)
                        {
                            if (ImGui::TreeNode(fmt::format("Vertex {}", j).c_str()))
                            {
                                ImGui::Text(fmt::format("Vertex pos: {}", (q + j)->vertex).c_str());
                                ImGui::Text(fmt::format("Colour: {}", (q + j)->colour).c_str());
                                ImGui::Text(fmt::format("Tex Coord: {}", (q + j)->texCoord).c_str());
                                ImGui::Text(fmt::format("Tex Size: {}", (q + j)->texSize).c_str());
                                ImGui::Text(fmt::format("Entity ID: {}", (q + j)->entityId).c_str());
                                ImGui::TreePop();
                            }
                        }
                        ImGui::TreePop();
                    }
                }
                ImGui::TreePop();
            }
        }
        ImGui::End();
    }
}

void EditorLayer::OnEvent(Event& e)
{
    EventDispatcher d(e);
    d.Dispatch<KeyDownEvent>(BindEventDelegate(this, &EditorLayer::OnKeyDown_Event));
}

bool EditorLayer::OnKeyDown_Event(KeyDownEvent& e)
{
    switch (e.GetKey())
    {
        case Key::Num1: m_GizmoMode = GizmoMode::Translation; return true;
        case Key::Num2: m_GizmoMode = GizmoMode::Rotation; return true;
        case Key::Num3: m_GizmoMode = GizmoMode::Scale; return true;
        default: break;
    }
    return false;
}
