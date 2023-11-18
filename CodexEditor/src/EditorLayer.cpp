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

    // Properties panel
    {
        ImGui::Begin("Entity properties");
        if (m_SelectedEntity.entity)
        {
            if (ImGui::Button("Add component"))
                ImGui::OpenPopup("component_popup");
            if (ImGui::BeginPopup("component_popup"))
            {
                if (ImGui::Button("Sprite Renderer Component"))
                {
                    m_SelectedEntity.entity.AddComponent<SpriteRendererComponent>(Sprite::Empty());
                    ImGui::CloseCurrentPopup();
                }
                else if (ImGui::Button("C++ Script Component"))
                {
                    m_SelectedEntity.entity.AddComponent<NativeBehaviourComponent>();
                    ImGui::CloseCurrentPopup();
                }
                ImGui::EndPopup();
            }

            if (m_SelectedEntity.entity.HasComponent<TransformComponent>())
            {
                if (ImGui::TreeNodeEx("Transform Component", ImGuiTreeNodeFlags_DefaultOpen))
                {
                    auto& c = m_SelectedEntity.entity.GetComponent<TransformComponent>();
                    DrawVec3Control("Position: ", c.position, m_ColumnWidth);
                    DrawVec3Control("Rotation: ", c.rotation, m_ColumnWidth);
                    DrawVec3Control("Scale: ", c.scale, m_ColumnWidth, 0.01f);

                    ImGui::TreePop();
                }
            }

            if (m_SelectedEntity.entity.HasComponent<NativeBehaviourComponent>())
            {
                if (ImGui::TreeNodeEx("C++ Script Component", ImGuiTreeNodeFlags_DefaultOpen))
                {
                    auto& c = m_SelectedEntity.entity.GetComponent<NativeBehaviourComponent>();

                    ImGui::Columns(2);
                    ImGui::SetColumnWidth(0, m_ColumnWidth);
                    ImGui::Text("Attach class: ");
                    ImGui::NextColumn();

                    ImGui::BeginGroup();
                    static NativeBehaviour* script         = nullptr;
                    static bool             invalid_script = false;
                    static std::string      script_class;
                    static std::string      data;
                    if (ImGui::InputText("##input", &script_class))
                    {
                        script         = nullptr;
                        invalid_script = true;
                    }
                    if (ImGui::IsKeyPressed(ImGuiKey_Enter)) // Check for Enter key press
                    {
                        invalid_script = true;
                        bool valid =
                            m_ScriptModule->Invoke<bool(const char*)>("Rf_DoesInstanceExist", script_class.c_str());
                        if (valid)
                        {
                            // TODO: This should happen OnScenePlay().
                            invalid_script = false;
                            script         = m_ScriptModule->Invoke<NativeBehaviour*(const char*)>("Rf_CreateInstance",
                                                                                           script_class.c_str());
                            script->SetOwner(m_SelectedEntity.entity);
                            c.Attach(script);
                        }
                    }
                    if (!script)
                    {
                        if (!invalid_script)
                            ImGui::Text("No bound class.");
                        else
                            ImGui::Text("No such class.");
                    }
                    else
                    {
                        ImGui::Text("libNBMan.dll : %s", script_class.c_str());
                    }
                    ImGui::EndGroup();
                    ImGui::Columns(1);

                    // Display serialized fields
                    for (auto& [k, v] : c.behaviours)
                    {
                        const auto& j = v->GetSerializedData();
                        if (j.empty())
                            v->Serialize();

                        const auto& klass = j.begin();
                        if (ImGui::TreeNodeEx(klass.key().c_str(), ImGuiTreeNodeFlags_DefaultOpen))
                        {
                            for (const auto& field : klass.value()["Fields"].items())
                            {
                                // fmt::println("\n{}", field.value().dump());
                                const std::string& field_name = field.key();
                                const FieldType    field_type = field.value().at("Type");

                                ImGui::Columns(2);
                                ImGui::SetColumnWidth(0, m_ColumnWidth);
                                ImGui::Text(field_name.c_str());
                                ImGui::NextColumn();

                                object field_ptr = v->GetField(field_name);
                                switch (field_type)
                                {
                                    case FieldType::I32:
                                    case FieldType::U32: {
                                        ImGui::DragInt("##drag_int", (i32*)field_ptr);
                                        break;
                                    }
                                    case FieldType::F32:
                                    case FieldType::F64:
                                    case FieldType::F128: {
                                        ImGui::DragFloat("##dragger", (f32*)field_ptr);
                                        break;
                                    }
                                    case FieldType::CString: {
                                        break;
                                    }
                                    case FieldType::StdString: {
                                        ImGui::InputText("##input", (std::string*)field_ptr);
                                        break;
                                    }
                                    case FieldType::Boolean: {
                                        ImGui::Checkbox("##checkbox", (bool*)field_ptr);
                                        break;
                                    }
                                    case FieldType::Vector2f: {
                                        DrawVec2Control("##t", *(Vector2f*)field_ptr, m_ColumnWidth);
                                        break;
                                    }
                                    case FieldType::Vector3f: {
                                        DrawVec3Control("##t", *(Vector3f*)field_ptr, m_ColumnWidth);
                                        break;
                                    }
                                    default: cx_throw(CodexException, "WHAT THE FUCK"); break;
                                }
                                ImGui::Columns(1);
                            }
                            for (const auto& field_obj : klass.value()["Fields"])
                            {
                                /*
                                fmt::println("{}"field_obj.dump();
                                const auto& field = field_obj.begin();

                                ImGui::Columns(2);
                                ImGui::SetColumnWidth(0, m_ColumnWidth);
                                ImGui::Text(field.key().c_str());
                                ImGui::NextColumn();

                                static std::string field_data = field.value().dump();
                                ImGui::InputText("##input", &field_data);

                                ImGui::Columns(1);
                                */
                            }
                            ImGui::TreePop();
                        }
                    }
                    ImGui::TreePop();
                }
            }

            if (m_SelectedEntity.entity.HasComponent<SpriteRendererComponent>())
            {
                if (ImGui::TreeNodeEx("Sprite Renderer Component", ImGuiTreeNodeFlags_DefaultOpen))
                {
                    auto& c       = m_SelectedEntity.entity.GetComponent<SpriteRendererComponent>();
                    auto& sprite  = c.GetSprite();
                    auto  texture = sprite.GetTexture();

                    // Texture prewview image
                    ImGui::Columns(2);
                    ImGui::SetColumnWidth(0, m_ColumnWidth);
                    ImGui::Text("Texture: ");
                    ImGui::NextColumn();

                    ImGui::BeginGroup();
                    if (sprite)
                        ImGui::Image((void*)(intptr)texture->GetGlId(), { 100.0f, 100.0f }, { 0, 1 }, { 1, 0 });
                    else
                        ImGui::Text("No bound texture.");

                    if (ImGui::Button("Load texture", { 100, 0 }))
                    {
                        const char* filters[] = { "*.png", "*.jpg" };
                        const char* file = tinyfd_openFileDialog("Load texture file.", nullptr, 2, filters, nullptr, 0);
                        if (file)
                        {
                            std::filesystem::path relative_path =
                                std::filesystem::relative(file, std::filesystem::current_path());
                            auto res = Resources::Load<Texture2D>(relative_path.string());
                            sprite.SetTexture(res);
                            m_SelectedEntity.overlayColour = sprite.GetColour();
                            sprite.GetColour()             = m_SelectColour;
                        }
                    }
                    ImGui::EndGroup();
                    ImGui::Columns(1);
                    ImGui::Dummy(ImVec2(0.0f, 10.0f));

                    // Texture coordinates
                    auto&    tex_coords = sprite.GetTextureCoords();
                    Vector2f pos        = { tex_coords.x, tex_coords.y };
                    Vector2f size       = { tex_coords.w, tex_coords.h };
                    DrawVec2Control("Texture position: ", pos, m_ColumnWidth);
                    DrawVec2Control("Texture size: ", size, m_ColumnWidth);
                    tex_coords = { pos.x, pos.y, size.x, size.y };

                    // Texture filter mode
                    if (texture)
                    {
                        ImGui::Dummy(ImVec2(0.0f, 10.0f));
                        ImGui::Columns(2);
                        ImGui::SetColumnWidth(0, m_ColumnWidth);
                        ImGui::Text("Texture filter mode: ");
                        ImGui::NextColumn();
                        static int  item_current_idx = 0; // Here we store our selection data as an index.
                        const char* preview_item     = nullptr;
                        const auto& props            = texture->GetProperties();
                        switch (props.filterMode)
                        {
                            case TextureFilterMode::Linear: preview_item = "Linear"; break;
                            case TextureFilterMode::Nearest: preview_item = "Nearest"; break;
                        }
                        if (ImGui::BeginCombo("##texture_filter_mode", preview_item))
                        {
                            if (ImGui::Selectable("Nearest", props.filterMode == TextureFilterMode::Nearest))
                            {
                                if (props.filterMode != TextureFilterMode::Nearest)
                                {
                                    auto new_props       = props;
                                    new_props.filterMode = TextureFilterMode::Nearest;
                                    texture->New(texture->GetFilePath(), new_props);
                                }
                            }
                            if (ImGui::Selectable("Linear", props.filterMode == TextureFilterMode::Linear))
                            {
                                if (props.filterMode != TextureFilterMode::Linear)
                                {
                                    auto new_props       = props;
                                    new_props.filterMode = TextureFilterMode::Linear;
                                    texture->New(texture->GetFilePath(), new_props);
                                }
                            }
                            ImGui::EndCombo();
                        }
                        ImGui::Columns(1);
                    }

                    ImGui::Dummy(ImVec2(0.0f, 10.0f));
                    auto sprite_size = sprite.GetSize();
                    DrawVec2Control("Sprite size: ", sprite_size, m_ColumnWidth);
                    sprite.SetSize(sprite_size);

                    // Texture colour picker
                    ImGui::Dummy(ImVec2(0.0f, 10.0f));
                    ImGui::Columns(2);
                    ImGui::SetColumnWidth(0, m_ColumnWidth);
                    ImGui::Text("Sprite overlay: ");
                    ImGui::NextColumn();

                    auto&               colour = m_SelectedEntity.overlayColour;
                    ImGuiColorEditFlags flags  = 0;
                    flags |= ImGuiColorEditFlags_AlphaBar;
                    flags |= ImGuiColorEditFlags_DisplayRGB; // Override display mode
                    f32 temp_colour[4]{ colour.x, colour.y, colour.z, colour.w };
                    ImGui::ColorPicker4("##color_picker", temp_colour, flags);
                    colour = { temp_colour[0], temp_colour[1], temp_colour[2], temp_colour[3] };
                    ImGui::Columns(1);

                    ImGui::Dummy(ImVec2(0.0f, 10.0f));
                    ImGui::Columns(2);
                    ImGui::SetColumnWidth(0, m_ColumnWidth);
                    ImGui::Text("Z Index: ");
                    ImGui::NextColumn();
                    ImGui::DragInt("##drag_int", &sprite.GetZIndex());
                    ImGui::Columns(1);

                    ImGui::TreePop();
                }
            }
        }

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

void EditorLayer::DrawVec3Control(const char* label, Vector3f& values, const f32 columnWidth, const f32 speed,
                                  const f32 resetValue)
{
    ImGuiIO& io        = ImGui::GetIO();
    auto     bold_font = io.Fonts->Fonts[0];

    ImGui::PushID(label);

    if (label[0] != '#' && label[1] != '#')
    {
        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, columnWidth);
        ImGui::Text(label);
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

void EditorLayer::DrawVec2Control(const char* label, Vector2f& values, const f32 columnWidth, const f32 speed,
                                  const f32 resetValue)
{
    ImGuiIO& io        = ImGui::GetIO();
    auto     bold_font = io.Fonts->Fonts[0];

    ImGui::PushID(label);

    if (label[0] != '#' && label[1] != '#')
    {
        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, columnWidth);
        ImGui::Text(label);
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
